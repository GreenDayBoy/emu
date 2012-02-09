#ifndef eMUCORE_SOCKET_HPP
#define eMUCORE_SOCKET_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../shared/types.hpp"
#include "buffer.hpp"
#include "log.hpp"

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

namespace eMUCore {
namespace network {

template<typename Service = boost::asio::io_service,
         typename Socket = boost::asio::ip::tcp::socket>
class socket_t: private boost::noncopyable {
public:
    class eventHandler_t {
    public:
        virtual ~eventHandler_t() {}
        virtual void onReceive(uint8 *payload, size_t size) = 0;
        virtual void onClose() = 0;
    };

    socket_t(Service &ioService,
             eventHandler_t &eventHandler):
      socket_(ioService),
      eventHandler_(eventHandler) {
        this->queueRead();
    }
    virtual ~socket_t() {}

    void close() {
        eventHandler_.onClose();
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close();
    }

    void send(const uint8 *payload, size_t size) {
        bool result = wbuf_.insert(payload, size);

        if(!result) {
            LOG_ERROR << "Cannot insert payload into write buffer." << std::endl;
            this->close();
            return;
        }

        if(!wbuf_.pending_) {
            wbuf_.pending_ = true;
            this->queueWrite();
        }
    }

private:
    socket_t();

    void queueRead() {
        socket_.async_receive(boost::asio::buffer(&rbuf_.payload_[0], maxPayloadSize_c),
                              boost::bind(&socket_t::receiveHandler,
                                          this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
    }

    void queueWrite() {
        // comment deleted by ACTA :-).
        socket_.async_write(boost::asio::buffer(&wbuf_.payload_[0], wbuf_.payloadSize_),
                            boost::bind(&socket_t::sendHandler,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred)); 
    }

    void receiveHandler(const boost::system::error_code& ec,
                        size_t bytesTransferred) {
        if(ec) {
            LOG_ERROR << "Error during handling receive operation, error: " << ec.message() << std::endl;
            this->close();
            return;
        }

        if(bytesTransferred > 0) {
            eventHandler_.onReceive(&rbuf_.payload_[0], bytesTransferred);
            this->queueRead();
        } else {
            this->close();
        }
    }

    void sendHandler(const boost::system::error_code& ec,
                     size_t bytesTransferred) {
        if(ec) {
            LOG_ERROR << "Error during handling sent operation, error: " << ec.message() << std::endl;
            this->close();
            return;
        }

        if(wbuf_.secPayloadSize_ > 0) {
            wbuf_.swap();
            this->queueWrite();
        }
        else
            wbuf_.clear();
    }

    Socket socket_;
    readBuffer_t rbuf_;
    writeBuffer_t wbuf_;
    eventHandler_t &eventHandler_;
};

}
}


#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif // eMUCORE_SOCKET_HPP
#ifndef eMUCORE_SOCKET_HPP
#define eMUCORE_SOCKET_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
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
class socket_t: private boost::noncopyable,
                public boost::enable_shared_from_this<socket_t<Service, Socket> > {
public:
    typedef boost::shared_ptr<socket_t<Service, Socket> > ptr_t;
    typedef boost::function1<void, typename socket_t<Service, Socket>::ptr_t> callback_t;

    socket_t(Service &ioService,
             const callback_t &readCallback,
             const callback_t &closeCallback):
      socket_(ioService),
      readCallback_(readCallback),
      closeCallback_(closeCallback) {}
    virtual ~socket_t() {}

    void close() {
        closeCallback_(shared_from_this());
        this->shutdown();
    }

    void shutdown() {
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

    Socket& impl() {
        return socket_;
    }

    void queueRead() {
        socket_.async_receive(boost::asio::buffer(&rbuf_.payload_[0], maxPayloadSize_c),
                              boost::bind(&socket_t::receiveHandler,
                                          this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
    }

    bool opened() {
        return socket_.is_open();
    }

    std::string address() {
        return socket_.remote_endpoint().address().to_string();
    }

    readBuffer_t &rbuf() { return rbuf_; }

private:
    socket_t();

    void queueWrite() {
        // comment deleted by ACTA :-).
        socket_.async_send(boost::asio::buffer(&wbuf_.payload_[0], wbuf_.payloadSize_),
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

        rbuf_.payloadSize_ = bytesTransferred;

        if(rbuf_.payloadSize_ > 0) {
            readCallback_(shared_from_this());
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
    callback_t readCallback_;
    callback_t closeCallback_;
};

}
}


#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif
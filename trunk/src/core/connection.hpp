#ifndef eMU_CORE_CONNECTION_HPP
#define eMU_CORE_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include "../shared/types.hpp"
#include "buffer.hpp"
#include "log.hpp"

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

namespace eMU {
namespace core {
namespace network {

template<typename SocketImpl = boost::asio::ip::tcp::socket,
         typename IoServiceImpl = boost::asio::io_service>
class connection_t: private boost::noncopyable {
public:
    class observer_i {
    public:
        virtual ~observer_i() {}
        virtual void receiveEvent(connection_t *connection, payload_t &payload) = 0;
        virtual void closeEvent(connection_t *connection) = 0;
    };

    connection_t(IoServiceImpl &ioService,
                 observer_i &observer):
      socket_(ioService),
      strand_(ioService),
      observer_(observer) {}

    virtual ~connection_t() {}

    SocketImpl& socket() { return socket_; }

    bool opened() { return socket_.is_open(); }

    std::string address() const { return socket_.remote_endpoint().address().to_string(); }

    void disconnect() {
        observer_.closeEvent(this);
        this->close();
    }

    void close() {
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close();
    }

    void send(const uint8 *payload, size_t size) {
        bool result = wbuf_.insert(payload, size);

        if(!result) {
            this->errorHandler(boost::asio::error::no_buffer_space, "send");
            return;
        }

        if(!wbuf_.pending_) {
            wbuf_.pending_ = true;
            this->queueSend();
        }
    }

    void queueReceive() {
        socket_.async_receive(boost::asio::buffer(&rbuf_.payload_[0], maxPayloadSize_c),
                              strand_.wrap(boost::bind(&connection_t::receiveHandler,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred)));
    }

private:
    connection_t();

    void queueSend() {
        // comment deleted by ACTA :-).
        socket_.async_send(boost::asio::buffer(&wbuf_.payload_[0], wbuf_.payloadSize_),
                            strand_.wrap(boost::bind(&connection_t::sendHandler,
                                                     this,
                                                     boost::asio::placeholders::error,
                                                     boost::asio::placeholders::bytes_transferred))); 
    }

    void receiveHandler(const boost::system::error_code& ec,
                        size_t bytesTransferred) {
        if(boost::asio::error::eof == ec) {
            this->disconnect();
            return;
        } else if(ec) {
            this->errorHandler(ec, "receive");
            return;
        }

        payload_t payload(rbuf_.payload_.begin(), rbuf_.payload_.begin() + bytesTransferred);
        this->queueReceive();
        observer_.receiveEvent(this, payload);
    }

    void sendHandler(const boost::system::error_code& ec,
                     size_t bytesTransferred) {
        if(ec) {
            this->errorHandler(ec, "send");
            return;
        }

        if(wbuf_.secPayloadSize_ > 0) {
            wbuf_.swap();
            this->queueSend();
        }
        else
            wbuf_.clear();
    }

    void errorHandler(const boost::system::error_code &ec, const std::string &operationName) {
        if(boost::asio::error::operation_aborted == ec) {
            return;
        }

        LOG_ERROR << "Error during handling async operation: " << operationName 
                  << ", error: " << ec.message() 
                  << ", code: " << ec.value() << std::endl;

        this->disconnect();
    }

    SocketImpl socket_;
    readBuffer_t rbuf_;
    writeBuffer_t wbuf_;
    typename IoServiceImpl::strand strand_;
    observer_i &observer_;
};

}
}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif
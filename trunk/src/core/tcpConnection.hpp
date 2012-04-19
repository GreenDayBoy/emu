#ifndef eMU_CORE_TCPCONNECTION_HPP
#define eMU_CORE_TCPCONNECTION_HPP

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
namespace tcp {

template<typename SocketImpl = boost::asio::ip::tcp::socket,
         typename IoServiceImpl = boost::asio::io_service>
class connection_t: private boost::noncopyable {
public:
    typedef boost::function1<void, connection_t*> connectEventCallback_t;
    typedef connectEventCallback_t closeEventCallback_t;
    typedef boost::function2<void, connection_t*, payload_t&> receiveEventCallback_t;

    connection_t(IoServiceImpl &ioService):
      socket_(ioService),
      strand_(ioService),
      closeOngoing_(false) {}

    virtual ~connection_t() {}

    SocketImpl& socket() { return socket_; }
    bool opened() { return socket_.is_open(); }
    std::string address() const { return socket_.remote_endpoint().address().to_string(); }

    void connectEventCallback(const connectEventCallback_t &callback) { connectEventCallback_ = callback; }
    void receiveEventCallback(const receiveEventCallback_t &callback) { receiveEventCallback_ = callback; }
    void closeEventCallback(const closeEventCallback_t &callback) { closeEventCallback_ = callback; }

    void disconnect() {
        closeOngoing_ = true;

        IoServiceImpl &ioService = socket_.get_io_service();
        ioService.post(strand_.wrap(boost::bind(closeEventCallback_, this)));
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

    void connect(const boost::asio::ip::tcp::endpoint &endpoint) {
        socket_.async_connect(endpoint,
                              boost::bind(&connection_t::connectHandler,
                                          this,
                                          boost::asio::placeholders::error));
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
        receiveEventCallback_(this, payload);

        if(!closeOngoing_) {
            this->queueReceive();
        }
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

    void connectHandler(const boost::system::error_code &ec) {
        if(ec) {
            this->errorHandler(ec, "connect");
            return;
        }

        connectEventCallback_(this);
    }

    SocketImpl socket_;
    readBuffer_t rbuf_;
    writeBuffer_t wbuf_;
    typename IoServiceImpl::strand strand_;
    bool closeOngoing_;
    connectEventCallback_t connectEventCallback_;
    receiveEventCallback_t receiveEventCallback_;
    closeEventCallback_t closeEventCallback_;
};

}
}
}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif
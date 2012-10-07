#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <network/buffer.hpp>
#include <network/udp/udpWriteBufferManager.hpp>
#include <common/log.hpp>

#ifdef eMU_UT
#include <ut/env/core/ioServiceStub.hpp>
#include <ut/env/core/udp/udpSocketMock.hpp>
#endif

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

namespace eMU {
namespace core {
namespace network {
namespace udp {

#ifdef eMU_UT
template<typename socketImpl = eMU::ut::env::core::udp::socketMock_t,
         typename ioServiceImpl = eMU::ut::env::core::ioServiceStub_t>
#else
template<typename socketImpl = boost::asio::ip::udp::socket,
         typename ioServiceImpl = boost::asio::io_service>
#endif
class connection_t: private boost::noncopyable {
public:
    typedef boost::function2<void, connection_t*, const boost::asio::ip::udp::endpoint&> receiveFromEventCallback_t;

    connection_t(ioServiceImpl &ioService, uint16 port):
      socket_(ioService,
              boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      strand_(ioService) {}

    virtual ~connection_t() {}

    readBuffer_t &rbuf() { return rbuf_; }
    socketImpl& socket() { return socket_; }
    void receiveFromEventCallback(const receiveFromEventCallback_t &callback) { receiveFromEventCallback_ = callback; }

    void queueReceiveFrom() {
        socket_.async_receive_from(boost::asio::buffer(&rbuf_.payload_[0], maxPayloadSize_c),
                                   senderEndpoint_,
                                   strand_.wrap(boost::bind(&connection_t::receiveFromHandler,
                                                            this,
                                                            boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred)));
    }

    void sendTo(const boost::asio::ip::udp::endpoint &endpoint, uint8 *payload, size_t size) {
        writeBuffer_t *wbuf = wbufManager_.get(endpoint);

        bool result = wbuf->insert(payload, size);

        if(!result) {
            this->errorHandler(boost::asio::error::no_buffer_space, "sendTo");
            return;
        }

        if(!wbuf->pending_) {
            wbuf->pending_ = true;
            this->queueSendTo(endpoint, wbuf);
        }
    }

private:
    connection_t();

    void receiveFromHandler(const boost::system::error_code& ec, size_t bytesTransferred) {
       if(ec) {
            this->errorHandler(ec, "receiveFrom");
        } else {
            rbuf_.payloadSize_ = bytesTransferred;
            receiveFromEventCallback_(this, senderEndpoint_);
        }

        this->queueReceiveFrom();
    }

    void queueSendTo(const boost::asio::ip::udp::endpoint &endpoint, writeBuffer_t *wbuf) {
        // comment deleted by ACTA :-).
        socket_.async_send_to(boost::asio::buffer(&wbuf->payload_[0], wbuf->payloadSize_),
                              endpoint,
                              strand_.wrap(boost::bind(&connection_t::sendToHandler,
                                                       this,
                                                       endpoint,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred))); 
    }

    void sendToHandler(const boost::asio::ip::udp::endpoint &endpoint,
                       const boost::system::error_code& ec,
                       size_t bytesTransferred) {
        if(ec) {
            this->errorHandler(ec, "sendTo");
        }

        writeBuffer_t *wbuf = wbufManager_.get(endpoint);

        if(wbuf->secPayloadSize_ > 0) {
            wbuf->swap();
            this->queueSendTo(endpoint, wbuf);
        }
        else
            wbufManager_.free(endpoint);
    }

    void errorHandler(const boost::system::error_code &ec, const std::string &operationName) {
        LOG_ERROR << "Error during handling async operation: " << operationName 
                  << ", error: " << ec.message() 
                  << ", code: " << ec.value() << std::endl;
    }

    socketImpl socket_;
    readBuffer_t rbuf_;
    writeBufferManager_t wbufManager_;
    typename ioServiceImpl::strand strand_;
    boost::asio::ip::udp::endpoint senderEndpoint_;
    receiveFromEventCallback_t receiveFromEventCallback_;
};

}
}
}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

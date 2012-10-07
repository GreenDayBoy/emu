#include <env/core/udp/udpSocketMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace udp {

socketMock_t::socketMock_t(ioServiceStub_t &ioService,
                           const boost::asio::ip::udp::endpoint &endpoint):
  rbuf_(NULL),
  rbufSize_(0),
  senderEndpoint_(NULL) {
    ON_CALL(*this, async_receive_from(::testing::_,
                                      ::testing::_,
                                      ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                                     &socketMock_t::impl_async_receive_from));

    ON_CALL(*this, async_send_to(::testing::_,
                                 ::testing::_,
                                 ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                                &socketMock_t::impl_async_send_to));
}

void socketMock_t::expectCall_async_receive_from() {
    EXPECT_CALL(*this, async_receive_from(::testing::_, ::testing::_, ::testing::_));
}

void socketMock_t::impl_async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                                                              boost::asio::ip::udp::endpoint &senderEndpoint,
                                                              const ioServiceStub_t::ioHandler_t &handler) {
    rbuf_ = boost::asio::buffer_cast<uint8*>(buffer);
    rbufSize_ = boost::asio::buffer_size(buffer);
    receiveFromHandler_ = handler;
    senderEndpoint_ = &senderEndpoint;
}

void socketMock_t::expectCall_async_send_to() {
    EXPECT_CALL(*this, async_send_to(::testing::_, ::testing::_, ::testing::_));
}

void socketMock_t::impl_async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                                                         boost::asio::ip::udp::endpoint endpoint,
                                                         const ioServiceStub_t::ioHandler_t &handler) {
    wbuf_ = boost::asio::buffer_cast<uint8*>(buffer);
    wbufSize_ = boost::asio::buffer_size(buffer);
    sendToHandler_ = handler;
    endpoint_ = &endpoint;
}

}
}
}
}
}
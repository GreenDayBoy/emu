#pragma once

#include <gmock/gmock.h>
#include <boost/asio.hpp>

#include <shared/types.hpp>
#include <env/core/ioServiceStub.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace udp {

class socketMock_t: private boost::noncopyable {
public:
    socketMock_t(ioServiceStub_t &ioService,
                    const boost::asio::ip::udp::endpoint &endpoint);

    MOCK_METHOD3(async_receive_from, void(boost::asio::mutable_buffers_1 &buffer,
                                          boost::asio::ip::udp::endpoint &senderEndpoint,
                                          const ioServiceStub_t::ioHandler_t &handler));
    void expectCall_async_receive_from();

    void impl_async_receive_from(boost::asio::mutable_buffers_1 &buffer,
                                 boost::asio::ip::udp::endpoint &senderEndpoint,
                                 const ioServiceStub_t::ioHandler_t &handler);

    MOCK_METHOD3(async_send_to, void(boost::asio::mutable_buffers_1 &buffer,
                                     boost::asio::ip::udp::endpoint endpoint,
                                     const ioServiceStub_t::ioHandler_t &handler));
    void expectCall_async_send_to();

    void impl_async_send_to(boost::asio::mutable_buffers_1 &buffer,
                            boost::asio::ip::udp::endpoint endpoint,
                            const ioServiceStub_t::ioHandler_t &handler);


    uint8 *rbuf_;
    size_t rbufSize_;
    ioServiceStub_t::ioHandler_t receiveFromHandler_;
    boost::asio::ip::udp::endpoint *senderEndpoint_;

    uint8 *wbuf_;
    size_t wbufSize_;
    ioServiceStub_t::ioHandler_t sendToHandler_;
    boost::asio::ip::udp::endpoint *endpoint_;
};

}
}
}
}
}

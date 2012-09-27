#pragma once

#include <gmock/gmock.h>
#include <boost/asio/ip/udp.hpp>

#include <core/network/buffer.hpp>
#include <core/network/udp/udpConnection.hpp>
#include <env/core/ioServiceStub.hpp>
#include <env/core/udp/udpSocketMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace udp {

class connectionEventCallbacksMock_t {
public:
    MOCK_METHOD2(receiveFromEvent, void(eMU::core::network::udp::connection_t<> *connection,
                                        const boost::asio::ip::udp::endpoint &endpoint));

    void expectCall_receiveFromEvent(eMU::core::network::udp::connection_t<> *connection,
                                     const boost::asio::ip::udp::endpoint &endpoint);
};

}
}
}
}
}

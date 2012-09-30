#pragma once

#include <gmock/gmock.h>

#include <core/network/tcp/tcpConnection.hpp>
#include <env/core/ioServiceStub.hpp>
#include <env/core/tcp/tcpSocketMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

class connectionEventCallbacksMock_t {
public:
    MOCK_METHOD1(connectEvent, void(eMU::core::network::tcp::connection_t<> &connection));
    void expectCall_connectEvent(eMU::core::network::tcp::connection_t<> &connection);

    MOCK_METHOD1(receiveEvent, void(eMU::core::network::tcp::connection_t<> &connection));
    void expectCall_receiveEvent(eMU::core::network::tcp::connection_t<> &connection);

    MOCK_METHOD1(closeEvent, void(eMU::core::network::tcp::connection_t<> &connection));
    void expectCall_closeEvent(eMU::core::network::tcp::connection_t<> &connection);
};

}
}
}
}
}

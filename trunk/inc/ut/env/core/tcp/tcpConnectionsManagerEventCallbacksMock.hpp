#pragma once

#include <gmock/gmock.h>

#include <core/network/tcp/tcpConnection.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

class connectionsManagerEventCallbacksMock_t {
public:
    MOCK_METHOD1(acceptEvent, void(eMU::core::network::tcp::connection_t<> *connection));
    void expectCall_acceptEvent();

    eMU::core::network::tcp::connection_t<> *connection_;
};

}
}
}
}
}

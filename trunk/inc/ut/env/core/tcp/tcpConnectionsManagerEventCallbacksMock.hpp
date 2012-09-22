#ifndef __ut_tcpConnectionsManagerEventCallbacksMock_hpp__
#define __ut_tcpConnectionsManagerEventCallbacksMock_hpp__

#include <gmock/gmock.h>

#include <core/network/tcp/tcpConnection.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

class connectionsManagerEventCallbacksMock_t {
public:
    connectionsManagerEventCallbacksMock_t();

    MOCK_METHOD1(acceptEvent, void(eMU::core::network::tcp::connection_t<> *connection));
    void expectCall_acceptEvent();
    void impl_acceptEvent(eMU::core::network::tcp::connection_t<> *connection);

    eMU::core::network::tcp::connection_t<> *connection_;
};

}
}
}
}
}


#endif
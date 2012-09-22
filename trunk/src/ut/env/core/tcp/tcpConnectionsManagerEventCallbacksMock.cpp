#include <env/core/tcp/tcpConnectionsManagerEventCallbacksMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

connectionsManagerEventCallbacksMock_t::connectionsManagerEventCallbacksMock_t() {
    ON_CALL(*this, acceptEvent(::testing::_)).WillByDefault(::testing::Invoke(this, &connectionsManagerEventCallbacksMock_t::impl_acceptEvent));
}

void connectionsManagerEventCallbacksMock_t::expectCall_acceptEvent() {
    EXPECT_CALL(*this, acceptEvent(::testing::NotNull()));
}

void connectionsManagerEventCallbacksMock_t::impl_acceptEvent(eMU::core::network::tcp::connection_t<> *connection) {
    connection_ = connection;
}

}
}
}
}
}
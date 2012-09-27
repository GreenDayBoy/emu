#include <env/core/tcp/tcpConnectionsManagerEventCallbacksMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

void connectionsManagerEventCallbacksMock_t::expectCall_acceptEvent() {
    EXPECT_CALL(*this, acceptEvent(::testing::NotNull())).WillOnce(::testing::SaveArg<0>(&connection_));
}

}
}
}
}
}
#include <env/core/udp/udpConnectionEventCallbacksMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace udp {

void connectionEventCallbacksMock_t::expectCall_receiveFromEvent(eMU::core::network::udp::connection_t<> *connection,
                                                                 const boost::asio::ip::udp::endpoint &endpoint) {
    EXPECT_CALL(*this, receiveFromEvent(connection, endpoint));
}

}
}
}
}
}
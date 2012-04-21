#include "udpConnectionEventCallbacksMock.hpp"

namespace eMUNetworkUT = eMU::ut::network;

void eMUNetworkUT::udp::connectionEventCallbacksMock_t::expectCall_receiveFromEvent(testConnection_t *connection,
                                                                                    const boost::asio::ip::udp::endpoint &endpoint,
                                                                                    eMUNetwork::payload_t &payload) {
    EXPECT_CALL(*this, receiveFromEvent(connection, endpoint, testing::ContainerEq(payload)));
}
#include "tcpConnectionEventCallbacksMock.hpp"

namespace eMUNetworkUT = eMU::ut::network;

void eMUNetworkUT::tcp::connectionEventCallbacksMock_t::expectCall_connectEvent(testConnection_t *connection) {
    EXPECT_CALL(*this, connectEvent(connection));
}

void eMUNetworkUT::tcp::connectionEventCallbacksMock_t::expectCall_receiveEvent(testConnection_t *connection,
                                                                         eMUNetwork::payload_t &payload) {
    EXPECT_CALL(*this, receiveEvent(connection, testing::ContainerEq(payload)));
}

void eMUNetworkUT::tcp::connectionEventCallbacksMock_t::expectCall_closeEvent(testConnection_t *connection) {
    EXPECT_CALL(*this, closeEvent(connection));
}
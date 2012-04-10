#include "connectionObserverMock.hpp"

namespace eMUNetworkUT = eMU::ut::network;

void eMUNetworkUT::connectionObserverMock_t::expectCall_receiveEvent(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection,
                                                                  eMUNetwork::payload_t &payload) {
    EXPECT_CALL(*this, receiveEvent(connection, testing::ContainerEq(payload)));
}

void eMUNetworkUT::connectionObserverMock_t::expectCall_closeEvent(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection) {
    EXPECT_CALL(*this, closeEvent(connection));
}
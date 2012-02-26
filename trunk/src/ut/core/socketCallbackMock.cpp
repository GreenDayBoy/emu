#include "socketCallbackMock.hpp"

namespace eMUNetworkUT = eMUUnitTest::networkTest;
namespace eMUNetwork = eMUCore::network;

void eMUNetworkUT::socketCallbackMock_t::expectCall_onClose(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket) {
    EXPECT_CALL(*this, onClose(socket));
}

void eMUNetworkUT::socketCallbackMock_t::expectCall_onReceive(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket) {
    EXPECT_CALL(*this, onReceive(socket));
}
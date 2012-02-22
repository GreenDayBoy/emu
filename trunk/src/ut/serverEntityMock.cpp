#include "serverEntityMock.hpp"

namespace eMUNetworkUT = eMUUnitTest::networkTest;
namespace eMUNetwork = eMUCore::network;

void eMUNetworkUT::serverEntityMock_t::delegateMocks() {
    ON_CALL(*this, onPeerConnect(::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                                &serverEntityMock_t::onPeerConnectImpl));
}

eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t, eMUNetworkUT::socketStub_t>::ptr_t 
eMUNetworkUT::serverEntityMock_t::getConnectedSocket() {
    return socket_;
}

void eMUNetworkUT::serverEntityMock_t::onPeerConnectImpl(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t ptr) {
    socket_ = ptr;
}
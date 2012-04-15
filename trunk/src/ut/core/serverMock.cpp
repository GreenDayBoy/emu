#include "serverMock.hpp"

namespace eMUNetworkUT = eMU::ut::network;

eMUNetworkUT::serverMock_t::serverMock_t(ioServiceStub_t &ioService,
                                         uint16 port,
                                         size_t maxNumOfUsers):
  server_t(ioService,
           port,
           maxNumOfUsers) {
    ON_CALL(*this, onAccept(::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                            &serverMock_t::impl_onConnect));
}

void eMUNetworkUT::serverMock_t::expectCall_onAccept(bool retValue) {
    connectStatus_ = retValue;
    EXPECT_CALL(*this, onAccept(::testing::NotNull()));
}

void eMUNetworkUT::serverMock_t::expectCall_onReceive(userStub_t *user, eMU::core::network::payload_t &payload) {
    EXPECT_CALL(*this, onReceive(user, testing::ContainerEq(payload)));
}

void eMUNetworkUT::serverMock_t::expectCall_onClose(userStub_t *user) {
    EXPECT_CALL(*this, onClose(user));
}

bool eMUNetworkUT::serverMock_t::impl_onConnect(userStub_t *user) {
    user_ = user;
    return connectStatus_;
}
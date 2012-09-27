#include <env/core/serverMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {

serverMock_t::serverMock_t(ioServiceStub_t &ioService, uint16 port, size_t maxNumOfUsers):
  server_t(ioService, port, maxNumOfUsers),
  user_(NULL) {
}

void serverMock_t::expectCall_onAccept(bool retValue) {
    EXPECT_CALL(*this, onAccept(::testing::NotNull())).WillOnce(DoAll(::testing::SaveArg<0>(&user_), ::testing::Return(retValue)));
}

void serverMock_t::expectCall_onReceive(user::userStub_t *user) {
    EXPECT_CALL(*this, onReceive(user));
}

void serverMock_t::expectCall_onClose(user::userStub_t *user) {
    EXPECT_CALL(*this, onClose(user));
}

}
}
}
}
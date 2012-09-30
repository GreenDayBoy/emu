#include <env/core/serverMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {

serverMock_t::serverMock_t(ioServiceStub_t &ioService, uint16 port, size_t maxNumOfUsers):
  server_t(ioService, port, maxNumOfUsers),
  user_(NULL),
  acceptStatus_(true) {
}

void serverMock_t::expectCall_onAccept(bool retValue) {
    EXPECT_CALL(*this, onAccept(::testing::_));
}

bool serverMock_t::impl_onAccept(user::userStub_t &user) {
    user_= &user;
    return acceptStatus_;
}

void serverMock_t::expectCall_onReceive(user::userStub_t &user) {
    EXPECT_CALL(*this, onReceive(::testing::Ref(user)));
}

void serverMock_t::expectCall_onClose(user::userStub_t &user) {
    EXPECT_CALL(*this, onClose(::testing::Ref(user)));
}

}
}
}
}
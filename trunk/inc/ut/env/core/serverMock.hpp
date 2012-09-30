#pragma once

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <env/core/user/userStub.hpp>
#include <core/network/server.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {

class serverMock_t: public eMU::core::network::server_t<user::userStub_t> {
public:
    serverMock_t(ioServiceStub_t &ioService, uint16 port, size_t maxNumOfUsers);

    MOCK_METHOD1(onAccept, bool(user::userStub_t &user));
    void expectCall_onAccept();
    bool impl_onAccept(user::userStub_t &user);

    MOCK_METHOD1(onReceive, void(user::userStub_t &user));
    void expectCall_onReceive(user::userStub_t &user);

    MOCK_METHOD1(onClose, void(user::userStub_t &user));
    void expectCall_onClose(user::userStub_t &user);

    eMU::core::network::tcp::connectionsManager_t<>& connectionsManager() { return connectionsManager_; }

    user::userStub_t *user_;
    bool acceptStatus_;
};

}
}
}
}

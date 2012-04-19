#ifndef eMU_UT_SERVERMOCK_HPP
#define eMU_UT_SERVERMOCK_HPP

#include <gmock/gmock.h>

#include "userStub.hpp"
#include "socketMock.hpp"
#include "ioServiceStub.hpp"
#include "acceptorMock.hpp"
#include "../../core/server.hpp"

namespace eMU {
namespace ut {
namespace network {

class serverMock_t: public eMU::core::network::server_t<userStub_t,
                                                        eMU::core::network::tcp::connection_t<socketMock_t, ioServiceStub_t>,
                                                        ioServiceStub_t,
                                                        acceptorMock_t> {
public:
    serverMock_t(ioServiceStub_t &ioService,
                 uint16 port,
                 size_t maxNumOfUsers);

    MOCK_METHOD1(onAccept, bool(userStub_t *user));
    MOCK_METHOD2(onReceive, void(userStub_t *user, eMU::core::network::payload_t &payload));
    MOCK_METHOD1(onClose, void(userStub_t *user));

    void onStartup() {}
    void onCleanup() {}

    void expectCall_onAccept(bool retValue);
    void expectCall_onReceive(userStub_t *user, eMU::core::network::payload_t &payload);
    void expectCall_onClose(userStub_t *user);

    bool impl_onConnect(userStub_t *user);

    acceptorMock_t& acceptor() { return acceptor_; }

    void associateConnection(eMU::core::network::tcp::connection_t<socketMock_t, ioServiceStub_t> *connection);

    userStub_t *user_;
    bool connectStatus_;
};

}
}
}

#endif
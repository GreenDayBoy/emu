#ifndef eMUUT_SERVERENTITYMOCK_HPP
#define eMUUT_SERVERENTITYMOCK_HPP

#include "../core/serverEntity.hpp"
#include "../core/socket.hpp"
#include "ioServiceMock.hpp"
#include "socketStub.hpp"

namespace eMUUnitTest {
namespace networkTest {

namespace eMUNetwork = eMUCore::network;

class serverEntityMock_t: public eMUNetwork::serverEntity_t<eMUNetwork::socket_t<ioServiceMock_t,
                                                                                 socketStub_t> > {
public:
    void delegateMocks();
    eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t getConnectedSocket();

    MOCK_METHOD1(onPeerConnect, void(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t ptr));
    MOCK_METHOD1(onPeerReceive, void(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t ptr));
    MOCK_METHOD1(onPeerClose, void(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t ptr));

private:
    void onPeerConnectImpl(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t ptr);

    eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket_;
};

}
}

#endif
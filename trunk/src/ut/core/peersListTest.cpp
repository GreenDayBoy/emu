#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../core/peersList.hpp"
#include "ioServiceMock.hpp"
#include "socketStub.hpp"
#include "socketCallbackMock.hpp"

namespace eMUNetwork = eMUCore::network;
namespace eMUNetworkUT = eMUUnitTest::networkTest;

class peersList_t: public ::testing::Test {
public:
    typedef eMUNetwork::peer_t<eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                                    eMUNetworkUT::socketStub_t>>::ptr_t peerPtr_t;
    typedef eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                 eMUNetworkUT::socketStub_t>::ptr_t socketPtr_t;
    void SetUp() {
    }

    void TearDown() {}

    socketPtr_t createNewSocket() {
        socketPtr_t socket;

        socket.reset(new eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                               eMUNetworkUT::socketStub_t>(ioService_,
                                                                           boost::bind(&eMUNetworkUT::socketCallbackMock_t::onReceive,
                                                                                       &socketCallback_,
                                                                                       _1),
                                                                           boost::bind(&eMUNetworkUT::socketCallbackMock_t::onClose,
                                                                                       &socketCallback_,
                                                                                       _1)));

        return socket;
    }

    eMUNetwork::peersList_t<eMUNetwork::peer_t<eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                                                    eMUNetworkUT::socketStub_t>>> peersList_;
    ::testing::NiceMock<eMUNetworkUT::ioServiceMock_t> ioService_;
    eMUNetworkUT::socketCallbackMock_t socketCallback_;
    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket_;
};

TEST_F(peersList_t, initialize) {
    size_t numOfMaxUsers = 10;
    peersList_.initialize(numOfMaxUsers);

    size_t listSize = peersList_.size();
    ASSERT_EQ(numOfMaxUsers, listSize);
}

TEST_F(peersList_t, cleanup) {
    size_t numOfMaxUsers = 10;
    peersList_.initialize(numOfMaxUsers);

    peersList_.cleanup();

    size_t listSize = peersList_.size();
    ASSERT_EQ(0, listSize);
}

TEST_F(peersList_t, findFree) {
    size_t numOfMaxUsers = 2;
    peerPtr_t nullPeer;
    peersList_.initialize(numOfMaxUsers);

    socketPtr_t socket1 = this->createNewSocket();    
    peerPtr_t peer1 = peersList_.findFree();
    ASSERT_NE(nullPeer, peer1); 
    peer1->socket(socket1);

    socketPtr_t socket2 = this->createNewSocket();
    peerPtr_t peer2 = peersList_.findFree();
    ASSERT_NE(nullPeer, peer2);
    peer2->socket(socket2);

    peerPtr_t peer3 = peersList_.findFree();
    ASSERT_EQ(nullPeer, peer3);
}

TEST_F(peersList_t, find) {
    size_t numOfMaxUsers = 2;
    peerPtr_t nullPeer;
    peersList_.initialize(numOfMaxUsers);

    socketPtr_t socket1 = this->createNewSocket();
    peerPtr_t peer1 = peersList_.findFree();
    ASSERT_NE(nullPeer, peer1); 
    peer1->socket(socket1);

    socketPtr_t socket2 = this->createNewSocket();
    peerPtr_t peer2 = peersList_.findFree();
    ASSERT_NE(nullPeer, peer2);
    peer2->socket(socket2);

    peerPtr_t foundPeer = peersList_.find(socket1);
    ASSERT_EQ(peer1, foundPeer);

    foundPeer = peersList_.find(socket2);
    ASSERT_EQ(peer2, foundPeer);

    socketPtr_t nullSocket;
    foundPeer = peersList_.find(nullSocket);
    ASSERT_EQ(nullPeer, foundPeer);
}
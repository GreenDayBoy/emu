#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../core/server.hpp"
#include "socketStub.hpp"
#include "serverEntityMock.hpp"
#include "ioServiceMock.hpp"
#include "acceptorStub.hpp"

namespace eMUNetwork = eMUCore::network;
namespace eMUNetworkUT = eMUUnitTest::networkTest;

class serverTest_t: public ::testing::Test {
public:
    static const uint16 port_c = 1524;

    serverTest_t():
      server_(ioService_, serverEntity_, port_c) {}

    void SetUp() {}

    void TearDown() {}

    ::testing::NiceMock<eMUNetworkUT::ioServiceMock_t> ioService_;
    eMUNetworkUT::serverEntityMock_t serverEntity_;
    eMUNetwork::server_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::acceptorStub_t,
                         eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                              eMUNetworkUT::socketStub_t> > server_;
};

TEST_F(serverTest_t, accept) {
    ioService_.delegateMocks();

    ioService_.expectCall_accept();
    server_.initialize();
    
    serverEntity_.expectCall_onPeerConnect();
    ioService_.expectCall_accept();
    ioService_.dequeueAccept(boost::system::error_code());
}

TEST_F(serverTest_t, accept_Error) {
    ioService_.delegateMocks();

    ioService_.expectCall_accept();
    server_.initialize();

    ioService_.expectCall_accept();
    ioService_.dequeueAccept(boost::asio::error::network_down);
}

TEST_F(serverTest_t, peer_Receive) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    ioService_.expectCall_accept();
    server_.initialize();

    serverEntity_.expectCall_onPeerConnect();
    ioService_.expectCall_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    socket->queueRead();

    serverEntity_.expectCall_onPeerReceive(socket);
    ioService_.dequeueRead(boost::system::error_code(), eMUNetwork::maxPayloadSize_c);
}

TEST_F(serverTest_t, peer_Close_byPeer) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    ioService_.expectCall_accept();
    server_.initialize();

    serverEntity_.expectCall_onPeerConnect();
    ioService_.expectCall_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    socket->queueRead();

    serverEntity_.expectCall_onPeerClose(socket);
    ioService_.dequeueRead(boost::system::error_code(), 0);
}

TEST_F(serverTest_t, peer_Close_byServer) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    ioService_.expectCall_accept();
    server_.initialize();

    serverEntity_.expectCall_onPeerConnect();
    ioService_.expectCall_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    serverEntity_.expectCall_onPeerClose(socket);
    socket->close();
}
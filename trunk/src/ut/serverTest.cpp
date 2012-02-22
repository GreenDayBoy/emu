#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../core/server.hpp"
#include "socketStub.hpp"
#include "serverEntityMock.hpp"
#include "ioServiceMock.hpp"
#include "acceptorStub.hpp"

namespace eMUNetwork = eMUCore::network;
namespace eMUNetworkUT = eMUUnitTest::networkTest;

class serverTest_t: public ::testing::Test {
public:
    serverTest_t():
      server_(ioService_, serverEntity_) {}

    void SetUp() {}

    void TearDown() {}

    void expectCall_ioService_accept() {
        EXPECT_CALL(ioService_, accept(::testing::_));
    }

    void expectCall_serverEntity_onPeerConnect() {
        EXPECT_CALL(serverEntity_, onPeerConnect(::testing::NotNull()));
    }
    
    void expectCall_serverEntity_onPeerReceive(eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                                                    eMUNetworkUT::socketStub_t>::ptr_t socket) {
        EXPECT_CALL(serverEntity_, onPeerReceive(socket));
    }

    void expectCall_serverEntity_onPeerClose(eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                                                  eMUNetworkUT::socketStub_t>::ptr_t socket) {
        EXPECT_CALL(serverEntity_, onPeerClose(socket));
    }

    ::testing::NiceMock<eMUNetworkUT::ioServiceMock_t> ioService_;
    eMUNetworkUT::serverEntityMock_t serverEntity_;
    eMUNetwork::server_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::acceptorStub_t,
                         eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                              eMUNetworkUT::socketStub_t> > server_;
};

TEST_F(serverTest_t, accept) {
    ioService_.delegateMocks();

    this->expectCall_ioService_accept();
    server_.initialize();
    
    this->expectCall_serverEntity_onPeerConnect();
    this->expectCall_ioService_accept();
    ioService_.dequeueAccept(boost::system::error_code());
}

TEST_F(serverTest_t, accept_Error) {
    ioService_.delegateMocks();

    this->expectCall_ioService_accept();
    server_.initialize();

    this->expectCall_ioService_accept();
    ioService_.dequeueAccept(boost::asio::error::network_down);
}

TEST_F(serverTest_t, peer_Receive) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    this->expectCall_ioService_accept();
    server_.initialize();

    this->expectCall_serverEntity_onPeerConnect();
    this->expectCall_ioService_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    socket->queueRead();

    this->expectCall_serverEntity_onPeerReceive(socket);
    ioService_.dequeueRead(boost::system::error_code(), eMUNetwork::maxPayloadSize_c);
}

TEST_F(serverTest_t, peer_Close_byPeer) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    this->expectCall_ioService_accept();
    server_.initialize();

    this->expectCall_serverEntity_onPeerConnect();
    this->expectCall_ioService_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    socket->queueRead();

    this->expectCall_serverEntity_onPeerClose(socket);
    ioService_.dequeueRead(boost::system::error_code(), 0);
}

TEST_F(serverTest_t, peer_Close_byServer) {
    ioService_.delegateMocks();
    serverEntity_.delegateMocks();

    this->expectCall_ioService_accept();
    server_.initialize();

    this->expectCall_serverEntity_onPeerConnect();
    this->expectCall_ioService_accept();
    ioService_.dequeueAccept(boost::system::error_code());

    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket = serverEntity_.getConnectedSocket();

    this->expectCall_serverEntity_onPeerClose(socket);
    socket->close();
}
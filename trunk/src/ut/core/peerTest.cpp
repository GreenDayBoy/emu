#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../core/peer.hpp"
#include "ioServiceMock.hpp"
#include "socketStub.hpp"
#include "socketCallbackMock.hpp"

namespace eMUNetwork = eMUCore::network;
namespace eMUNetworkUT = eMUUnitTest::networkTest;

class peerTest_t: public ::testing::Test {
public:
    peerTest_t():
      peer_(0) {}
    
    void SetUp() {
        this->createSocket();
    }

    void TearDown() {}

    void createSocket() {
        socket_.reset(new eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                               eMUNetworkUT::socketStub_t>(ioService_,
                                                                           boost::bind(&eMUNetworkUT::socketCallbackMock_t::onReceive,
                                                                                       &socketCallback_,
                                                                                       _1),
                                                                           boost::bind(&eMUNetworkUT::socketCallbackMock_t::onClose,
                                                                                       &socketCallback_,
                                                                                       _1)));
    }

    eMUNetworkUT::ioServiceMock_t ioService_;
    eMUNetworkUT::socketCallbackMock_t socketCallback_;
    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket_;
    eMUNetwork::peer_t<eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                                            eMUNetworkUT::socketStub_t>> peer_;
};

TEST_F(peerTest_t, construct) {
    ioService_.expectCall_read();
    peer_.socket(socket_);
}

TEST_F(peerTest_t, close) {
    ioService_.expectCall_read();
    peer_.socket(socket_);

    socketCallback_.expectCall_onClose(socket_);
    ioService_.expectCall_shutdownSocket();
    ioService_.expectCall_closeSocket();
    peer_.disconnect();
}
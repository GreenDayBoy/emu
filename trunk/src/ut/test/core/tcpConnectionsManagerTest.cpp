#include <gtest/gtest.h>

#include <env/core/tcp/tcpConnectionsManagerEventCallbacksMock.hpp>
#include <env/core/tcp/acceptorMock.hpp>
#include <core/network/tcp/tcpConnectionsManager.hpp>

class tcpConnectionsManagerTest_t: public ::testing::Test {
public:
    tcpConnectionsManagerTest_t():
      connectionsManager_(ioService_, 44405) {}

    void SetUp() {
        connectionsManager_.acceptEventCallback(boost::bind(&eMU::ut::env::core::tcp::connectionsManagerEventCallbacksMock_t::acceptEvent,
                                                            &connectionsManagerEvents_,
                                                            _1));
    }
    
    void TearDown() {}

    eMU::core::network::tcp::connectionsManager_t<> connectionsManager_;
    eMU::ut::env::core::ioServiceStub_t ioService_;
    eMU::ut::env::core::tcp::connectionsManagerEventCallbacksMock_t connectionsManagerEvents_;
};

TEST_F(tcpConnectionsManagerTest_t, accept) {
    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.queueAccept();

    connectionsManagerEvents_.expectCall_acceptEvent();
    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.acceptor().acceptHandler_(boost::system::error_code());
}

TEST_F(tcpConnectionsManagerTest_t, accept__error) {
    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.queueAccept();

    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.acceptor().acceptHandler_(boost::asio::error::already_started);
}

TEST_F(tcpConnectionsManagerTest_t, release) {
    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.queueAccept();

    connectionsManagerEvents_.expectCall_acceptEvent();
    connectionsManager_.acceptor().expectCall_async_accept();
    connectionsManager_.acceptor().acceptHandler_(boost::system::error_code());

    connectionsManagerEvents_.connection_->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connectionsManagerEvents_.connection_->socket().expectCall_close();
    connectionsManager_.release(*connectionsManagerEvents_.connection_);
}
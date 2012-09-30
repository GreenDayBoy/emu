#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <env/core/ioServiceStub.hpp>
#include <env/core/serverMock.hpp>

class serverTest_t: public ::testing::Test {
public:
    serverTest_t():
      server_(ioService_, 55901, maxNumOfUsers_) {}

    void SetUp() {}
    void TearDown() {}

    eMU::ut::env::core::serverMock_t server_;
    eMU::ut::env::core::ioServiceStub_t ioService_;

    static const size_t maxNumOfUsers_ = 5;
};

TEST_F(serverTest_t, onAccept) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, onAccept__failed) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.acceptStatus_ = false;
    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    server_.connectionsManager().acceptor().socket_->expectCall_close();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, onReceive) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onReceive(*user);
    user->connection().socket().expectCall_async_receive();
    user->connection().socket().receiveHandler_(boost::system::error_code(), 127);
}

TEST_F(serverTest_t, onReceive__unknown_socket) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    eMU::core::network::tcp::connection_t<> &connection = user->connection();

    eMU::core::network::tcp::connection_t<> fakeConnection(ioService_);
    user->connection(fakeConnection);

    connection.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection.socket().expectCall_close();
    connection.socket().receiveHandler_(boost::system::error_code(), 43);
}

TEST_F(serverTest_t, onClose) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onClose(*user);
    user->connection().socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection().socket().expectCall_close();
    user->connection().socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(serverTest_t, onClose__unknown_socket) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    eMU::core::network::tcp::connection_t<> &connection = user->connection();

    eMU::core::network::tcp::connection_t<> fakeConnection(ioService_);
    user->connection(fakeConnection);

    connection.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection.socket().expectCall_close();
    connection.socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(serverTest_t, reachedMaxNumberOfUsers) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    for(size_t i = 0; i < maxNumOfUsers_; ++i) {
        server_.expectCall_onAccept();
        server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
        server_.connectionsManager().acceptor().expectCall_async_accept();
        server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
    }

    size_t numOfAttempts = 5;

    for(size_t i = 0; i < numOfAttempts; ++i) {
        server_.connectionsManager().acceptor().socket_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        server_.connectionsManager().acceptor().socket_->expectCall_close();
        server_.connectionsManager().acceptor().expectCall_async_accept();
        server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
    }

    // 1. Disconnect one user - now we are under max num of users.
    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onClose(*user);
    user->connection().socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection().socket().expectCall_close();
    user->connection().socket().receiveHandler_(boost::asio::error::eof, 0);

    // 2. Accept new user - it should succeed. After this maximum is reached again.
    server_.expectCall_onAccept();
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    // 3. Try to accept another users - it should not be succeed due to reached max num of users.
    for(size_t i = 0; i < numOfAttempts; ++i) {
        server_.connectionsManager().acceptor().socket_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        server_.connectionsManager().acceptor().socket_->expectCall_close();
        server_.connectionsManager().acceptor().expectCall_async_accept();
        server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
    }
}

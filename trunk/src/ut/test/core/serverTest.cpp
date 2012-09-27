#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <env/core/ioServiceStub.hpp>
#include <env/core/serverMock.hpp>

class serverTest_t: public ::testing::Test {
public:
    serverTest_t():
      server_(ioService_, 55901, 5) {}

    void SetUp() {}
    void TearDown() {}

    eMU::ut::env::core::serverMock_t server_;
    eMU::ut::env::core::ioServiceStub_t ioService_;
};

TEST_F(serverTest_t, onAccept) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, onAccept__failed) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(false);
    server_.connectionsManager().acceptor().socket_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    server_.connectionsManager().acceptor().socket_->expectCall_close();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, onReceive) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onReceive(user);
    user->connection()->socket().expectCall_async_receive();
    user->connection()->socket().receiveHandler_(boost::system::error_code(), 127);
}

TEST_F(serverTest_t, onReceive__with_error) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onClose(user);
    user->connection()->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection()->socket().expectCall_close();
    user->connection()->socket().receiveHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(serverTest_t, OnReceive__unknown_socket) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    eMU::core::network::tcp::connection_t<> *connection = user->connection();
    user->connection(NULL);

    connection->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection->socket().expectCall_close();
    connection->socket().receiveHandler_(boost::system::error_code(), 43);
}

TEST_F(serverTest_t, onClose) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    server_.expectCall_onClose(user);
    user->connection()->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection()->socket().expectCall_close();
    user->connection()->socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(serverTest_t, OnClose__unknown_socket) {
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.queueAccept();

    server_.expectCall_onAccept(true);
    server_.connectionsManager().acceptor().socket_->expectCall_async_receive();
    server_.connectionsManager().acceptor().expectCall_async_accept();
    server_.connectionsManager().acceptor().acceptHandler_(boost::system::error_code());

    eMU::ut::env::core::user::userStub_t *user = server_.user_;
    ASSERT_THAT(user, ::testing::NotNull());

    eMU::core::network::tcp::connection_t<> *connection = user->connection();
    user->connection(NULL);

    connection->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection->socket().expectCall_close();
    connection->socket().receiveHandler_(boost::asio::error::eof, 0);
}

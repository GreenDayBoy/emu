#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "serverMock.hpp"
#include "connectionObserverMock.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;

class serverTest_t: public ::testing::Test {
public:
    serverTest_t():
      server_(ioServiceStub_, 44405, maxNumOfUsers_) {
        acceptorMock_ = &server_.acceptor();
    }

    void SetUp() {}
    
    void TearDown() {}

    eMUNetworkUT::ioServiceStub_t ioServiceStub_;
    eMUNetworkUT::acceptorMock_t *acceptorMock_;
    eMUNetworkUT::serverMock_t server_;    

    static const size_t maxNumOfUsers_ = 10;
};

TEST_F(serverTest_t, accept) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, accept__onConnect_failed) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return false so server should close connection.
    acceptorMock_->socket_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    acceptorMock_->socket_->expectCall_close();

    server_.expectCall_onConnect(false);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());
}

TEST_F(serverTest_t, accept__error) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::asio::error::already_connected);
}

TEST_F(serverTest_t, close__by_peer) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    eMUNetworkUT::userStub_t *user = server_.user_;
    
    server_.expectCall_onClose(user);
    user->connection()->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection()->socket().expectCall_close();
    user->connection()->socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(serverTest_t, close__by_server) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    eMUNetworkUT::userStub_t *user = server_.user_;

    server_.expectCall_onClose(user);
    user->connection()->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection()->socket().expectCall_close();
    user->connection()->disconnect();
}

TEST_F(serverTest_t, close__not_found_user) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    // Create new connection which will not be owned by any user.
    eMUNetwork::connection_t<eMUNetworkUT::socketMock_t, eMUNetworkUT::ioServiceStub_t> connection(ioServiceStub_, server_);
    
    // Associate connection with server.
    connection.socket().expectCall_async_receive();
    connection.queueReceive();

    // Important: Connection is not associated with any user object.
    // It should be close but without calling onClose event.
    connection.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection.socket().expectCall_close();
    connection.socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(serverTest_t, receive) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    eMUNetworkUT::userStub_t *user = server_.user_;

    // Insert prepared payload to connection rbuf.
    eMUNetwork::payload_t payload(100, 0x14);
    memcpy(user->connection()->socket().rbuf_, &payload[0], payload.size());

    user->connection()->socket().expectCall_async_receive();
    server_.expectCall_onReceive(user, payload);
    user->connection()->socket().receiveHandler_(boost::system::error_code(), payload.size());
}

TEST_F(serverTest_t, receive__error) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    eMUNetworkUT::userStub_t *user = server_.user_;

    server_.expectCall_onClose(user);
    user->connection()->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    user->connection()->socket().expectCall_close();
    user->connection()->socket().receiveHandler_(boost::asio::error::host_unreachable, 0);
}

TEST_F(serverTest_t, receive__not_found_user) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    // Create new connection which will not be owned by any user.
    eMUNetwork::connection_t<eMUNetworkUT::socketMock_t, eMUNetworkUT::ioServiceStub_t> connection(ioServiceStub_, server_);

    // Associate connection with server.
    connection.socket().expectCall_async_receive();
    connection.queueReceive();

    // Normal another receive queuing - it is not matter if connection is associated with user object.
    connection.socket().expectCall_async_receive();

    // Important: Connection is not associated with any user object.
    // It should be close but without calling onClose event and onReceive event.
    connection.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection.socket().expectCall_close();
    connection.socket().receiveHandler_(boost::system::error_code(), 100);
}

TEST_F(serverTest_t, receive__error_not_found_user) {
    acceptorMock_->expectCall_async_accept();
    server_.queueAccept();

    //onConnect event return true so server should queue receive for accepted socket.
    acceptorMock_->socket_->expectCall_async_receive();

    server_.expectCall_onConnect(true);
    acceptorMock_->expectCall_async_accept();
    acceptorMock_->acceptHandler_(boost::system::error_code());

    // Create new connection which will not be owned by any user.
    eMUNetwork::connection_t<eMUNetworkUT::socketMock_t, eMUNetworkUT::ioServiceStub_t> connection(ioServiceStub_, server_);

    // Associate connection with server.
    connection.socket().expectCall_async_receive();
    connection.queueReceive();

    // Important: Connection is not associated with any user object.
    // It should be close but without calling onClose event.
    connection.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection.socket().expectCall_close();
    connection.socket().receiveHandler_(boost::asio::error::connection_refused, 100);
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connectionsFactory.hpp>

using eMU::core::network::tcp::ConnectionsFactory;
using eMU::core::network::tcp::Connection;
namespace asioStub = eMU::ut::env::asioStub;
using asioStub::io_service;


class TcpConnectionsFactoryTest: public ::testing::Test
{
protected:
    TcpConnectionsFactoryTest():
        socket_(new asioStub::ip::tcp::socket(ioService_)) {}

    io_service ioService_;
    ConnectionsFactory connectionsFactory_;
    Connection::SocketPointer socket_;
};

TEST_F(TcpConnectionsFactoryTest, createConnectionWithSameHashTwiceShouldThrowException)
{
    size_t hash = 1234;
    Connection::SocketPointer socket1(new asioStub::ip::tcp::socket(ioService_));
    connectionsFactory_.create(hash, socket1);

    Connection::SocketPointer socket2(new asioStub::ip::tcp::socket(ioService_));

    ASSERT_THROW(connectionsFactory_.create(hash, socket2), ConnectionsFactory::AlreadyExistingConnectionException);
}

TEST_F(TcpConnectionsFactoryTest, create)
{
    size_t hash = 1234;
    Connection &connection = connectionsFactory_.create(hash, socket_);

    EXPECT_EQ(hash, connectionsFactory_.getHash(connection));
}

TEST_F(TcpConnectionsFactoryTest, destroy)
{
    size_t hash = 1234;
    connectionsFactory_.create(hash, socket_);

    connectionsFactory_.destroy(hash);

    connectionsFactory_.create(hash, socket_);
}

TEST_F(TcpConnectionsFactoryTest, destroyNotExistingConnectionShouldThrowException)
{
    ASSERT_THROW(connectionsFactory_.destroy(4321), ConnectionsFactory::UnknownConnectionException);
}

TEST_F(TcpConnectionsFactoryTest, get)
{
    size_t hash = 1234;
    Connection &connection = connectionsFactory_.create(hash, socket_);

    Connection &gotConnection = connectionsFactory_.get(hash);

    ASSERT_EQ(connection, gotConnection);
}

TEST_F(TcpConnectionsFactoryTest, getNotExisitngConnectionShouldThrowException)
{
    ASSERT_THROW(connectionsFactory_.get(4321), ConnectionsFactory::UnknownConnectionException);
}

TEST_F(TcpConnectionsFactoryTest, getHashForNotExistingConnectionShouldThrowException)
{
    Connection connection(socket_);
    ASSERT_THROW(connectionsFactory_.getHash(connection), ConnectionsFactory::UnknownConnectionException);
}

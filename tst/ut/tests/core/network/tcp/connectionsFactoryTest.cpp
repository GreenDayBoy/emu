#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connectionsFactory.hpp>
#include <core/network/tcp/exceptions.hpp>

namespace asioStub = eMU::ut::env::asioStub;
namespace network = eMU::core::network;

class TcpConnectionsFactoryTest: public ::testing::Test
{
public:
    asioStub::io_service ioService_;
    network::tcp::ConnectionsFactory connectionsFactory_;
};

TEST_F(TcpConnectionsFactoryTest, createConnectionWithSameHashTwiceShouldThrowException)
{
    size_t hash = 1234;
    network::tcp::Connection::SocketPointer socket1(new asioStub::ip::tcp::socket(ioService_));
    connectionsFactory_.create(hash, socket1);

    network::tcp::Connection::SocketPointer socket2(new asioStub::ip::tcp::socket(ioService_));

    bool exceptionThrown = false;

    try
    {
        connectionsFactory_.create(hash, socket2);
    }
    catch(network::tcp::exceptions::AlreadyExistingConnectionException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(TcpConnectionsFactoryTest, create)
{
    size_t hash = 1234;
    network::tcp::Connection::SocketPointer socket(new asioStub::ip::tcp::socket(ioService_));
    network::tcp::Connection &connection = connectionsFactory_.create(hash, socket);

    EXPECT_EQ(hash, connectionsFactory_.getHash(connection));
}

TEST_F(TcpConnectionsFactoryTest, destroy)
{
    size_t hash = 1234;
    network::tcp::Connection::SocketPointer socket(new asioStub::ip::tcp::socket(ioService_));
    connectionsFactory_.create(hash, socket);

    connectionsFactory_.destroy(hash);

    connectionsFactory_.create(hash, socket);
}

TEST_F(TcpConnectionsFactoryTest, destroyNotExistingConnectionShouldThrowException)
{
    bool exceptionThrown = false;

    try
    {
        connectionsFactory_.destroy(4321);
    }
    catch(eMU::core::network::tcp::exceptions::UnknownConnectionException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(TcpConnectionsFactoryTest, get)
{
    size_t hash = 1234;
    network::tcp::Connection::SocketPointer socket(new asioStub::ip::tcp::socket(ioService_));
    network::tcp::Connection &connection = connectionsFactory_.create(hash, socket);

    network::tcp::Connection &gotConnection = connectionsFactory_.get(hash);

    ASSERT_EQ(connection.hash(), gotConnection.hash());
}

TEST_F(TcpConnectionsFactoryTest, getNotExisitngConnectionShouldThrowException)
{
    bool exceptionThrown = false;

    try
    {
        connectionsFactory_.get(4321);
    }
    catch(eMU::core::network::tcp::exceptions::UnknownConnectionException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(TcpConnectionsFactoryTest, getHashForNotExistingConnectionShouldThrowException)
{
    network::tcp::Connection::SocketPointer socket(new asioStub::ip::tcp::socket(ioService_));
    network::tcp::Connection connection(socket);

    bool exceptionThrown = false;

    try
    {
        connectionsFactory_.getHash(connection);
    }
    catch(eMU::core::network::tcp::exceptions::UnknownConnectionException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

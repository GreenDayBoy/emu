#include <core/network/tcp/connectionsAcceptor.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using eMU::ut::env::asioStub::io_service;
using eMU::ut::env::asioStub::ip::tcp::acceptor;
using eMU::core::network::tcp::ConnectionsAcceptor;
using eMU::core::network::tcp::Connection;
using eMU::core::common::Factory;

using ::testing::_;
using ::testing::SaveArg;

class ConnectionsAcceptorTest: public ::testing::Test
{
protected:
    ConnectionsAcceptorTest():
        acceptor_(new acceptor(ioService_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 55960))),
        connectionsAcceptor_(connectionsFactory_, acceptor_)
    {
        connectionsAcceptor_.setAcceptEventCallback(std::bind(&ConnectionsAcceptorTest::acceptEvent, this, std::placeholders::_1));
    }

    MOCK_METHOD1(acceptEvent, void(Connection &connection));

    io_service ioService_;
    Factory<Connection> connectionsFactory_;
    ConnectionsAcceptor::AcceptorPointer acceptor_;
    ConnectionsAcceptor connectionsAcceptor_;
    acceptor::AcceptHandler acceptHandler_;
};


TEST_F(ConnectionsAcceptorTest, WhenConnectionWasAcceptedWithoutErrorThenAcceptEventShouldBeCalled)
{
    EXPECT_CALL(*acceptor_, async_accept(_, _)).WillOnce(SaveArg<1>(&acceptHandler_));
    connectionsAcceptor_.queueAccept();

    EXPECT_CALL(*acceptor_, async_accept(_, _));
    EXPECT_CALL(*this, acceptEvent(_));
    acceptHandler_(boost::system::error_code());

    ASSERT_EQ(1, connectionsFactory_.getObjects().size());
}

TEST_F(ConnectionsAcceptorTest, WhenConnectionWasAcceptedWithErrorThenAcceptEventShouldNotBeCalled)
{
    EXPECT_CALL(*acceptor_, async_accept(_, _)).WillOnce(SaveArg<1>(&acceptHandler_));
    connectionsAcceptor_.queueAccept();

    EXPECT_CALL(*acceptor_, async_accept(_, _));
    acceptHandler_(boost::asio::error::access_denied);

    ASSERT_TRUE(connectionsFactory_.getObjects().empty());
}

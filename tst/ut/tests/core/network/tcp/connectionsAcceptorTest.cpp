#include <core/network/tcp/connectionsAcceptor.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using eMU::ut::env::asioStub::io_service;
using eMU::ut::env::asioStub::ip::tcp::acceptor;
using eMU::core::network::tcp::ConnectionsAcceptor;
using eMU::core::network::tcp::Protocol;
using eMU::core::network::tcp::Connection;
using eMU::core::common::Factory;

using ::testing::_;
using ::testing::SaveArg;

// TODO: Duplicated!
ACTION_TEMPLATE(SaveArgToPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer))
{
    *pointer = &(::std::tr1::get<k>(args));
}

class ConnectionsAcceptorTest: public ::testing::Test
{
protected:
    ConnectionsAcceptorTest():
        connectionsAcceptor_(ioService_, 55960, protocol_) {}

    io_service ioService_;
    ConnectionsAcceptor connectionsAcceptor_;
    Protocol protocol_;
    acceptor::AcceptHandler acceptHandler_;
};


TEST_F(ConnectionsAcceptorTest, WhenConnectionWasAcceptedWithoutErrorThenAcceptEventShouldBeCalled)
{
    asio::ip::tcp::socket *incomingSocket;
    EXPECT_CALL(connectionsAcceptor_.getAcceptor(), async_accept(_, _)).WillOnce(DoAll(SaveArgToPointer<0>(&incomingSocket),
                                                                                       SaveArg<1>(&acceptHandler_)));
    connectionsAcceptor_.queueAccept();

    EXPECT_CALL(*incomingSocket, async_receive(_, _));
    EXPECT_CALL(connectionsAcceptor_.getAcceptor(), async_accept(_, _));
    acceptHandler_(boost::system::error_code());
}

TEST_F(ConnectionsAcceptorTest, WhenConnectionWasAcceptedWithErrorThenAcceptEventShouldNotBeCalled)
{
    EXPECT_CALL(connectionsAcceptor_.getAcceptor(), async_accept(_, _)).WillOnce(SaveArg<1>(&acceptHandler_));
    connectionsAcceptor_.queueAccept();

    EXPECT_CALL(connectionsAcceptor_.getAcceptor(), async_accept(_, _));
    acceptHandler_(boost::asio::error::access_denied);
}

TEST_F(ConnectionsAcceptorTest, WhenOperationAbortedOccuredThenAcceptShouldNotBeQueued)
{
    EXPECT_CALL(connectionsAcceptor_.getAcceptor(), async_accept(_, _)).WillOnce(SaveArg<1>(&acceptHandler_));
    connectionsAcceptor_.queueAccept();

    acceptHandler_(boost::asio::error::operation_aborted);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>
#include <ut/env/core/network/tcp/connectionEventsMock.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

using ::testing::SaveArg;
using ::testing::SetArgReferee;
using ::testing::Return;
using ::testing::Ref;
using ::testing::_;
using ::testing::Throw;

using eMU::ut::env::core::network::tcp::ConnectionEventsMock;
using eMU::ut::env::core::network::SamplePayloads;

using eMU::core::network::tcp::Connection;
using eMU::core::network::Payload;

namespace asioStub = eMU::ut::env::asioStub;

class TcpConnectionTest: public ::testing::Test
{
protected:
    TcpConnectionTest():
        socket_(new asioStub::ip::tcp::socket(ioService_)),
        connection_(socket_),
        endpoint_(boost::asio::ip::tcp::v4(), 55962) {}

    void SetUp()
    {
        connection_.setReceiveEventCallback(std::bind(&ConnectionEventsMock::receiveEvent, &connectionEvents_, std::placeholders::_1));
        connection_.setCloseEventCallback(std::bind(&ConnectionEventsMock::closeEvent, &connectionEvents_, std::placeholders::_1));
    }

    void TearDown() {}

    void expectAsyncReceiveCallAndSaveArguments()
    {
        EXPECT_CALL(*socket_, async_receive(_, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_), SaveArg<1>(&receiveHandler_)));
    }

    void expectAsyncSendCallAndSaveArguments()
    {
        EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer_), SaveArg<1>(&sendHandler_)));
    }

    asioStub::io_service ioService_;
    Connection::SocketPointer socket_;
    Connection connection_;
    boost::asio::ip::tcp::endpoint endpoint_;

    ConnectionEventsMock connectionEvents_;

    asioStub::io_service::IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;

    asioStub::io_service::IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;

    SamplePayloads samplePayloads_;
};

TEST_F(TcpConnectionTest, close)
{
    EXPECT_CALL(*socket_, shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(*socket_, close());

    connection_.close();
}

TEST_F(TcpConnectionTest, disconnectShouldTriggerCloseEvent)
{
    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    connection_.disconnect();
}

TEST_F(TcpConnectionTest, disconnectAtNotOpenedSocketShouldNotTriggerCloseEvent)
{
    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    connection_.disconnect();
}

TEST_F(TcpConnectionTest, receiveWithEofErrorShouldTriggerCloseEvent)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(TcpConnectionTest, receive)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    ASSERT_TRUE(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_) != nullptr);
    ASSERT_EQ(Payload::getMaxSize(), boost::asio::buffer_size(receiveBuffer_));

    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &samplePayloads_.payload1_[0], samplePayloads_.payload1_.getSize());

    EXPECT_CALL(*socket_, async_receive(_, _));
    EXPECT_CALL(connectionEvents_, receiveEvent(Ref(connection_)));

    receiveHandler_(boost::system::error_code(), samplePayloads_.payload1_.getSize());

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), connection_.getReadPayload().getSize());
    EXPECT_EQ(memcmp(&samplePayloads_.payload1_[0], &connection_.getReadPayload()[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(TcpConnectionTest, receiveErrorShouldTriggerCloseEvent)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, receiveErrorShouldNotTriggerCloseEventWhenSocketIsNotOpen)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, receiveWithOperationAbortedErrorShouldNotTriggerCloseEvent)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    receiveHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, send)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(samplePayloads_.payload2_);

    ASSERT_EQ(samplePayloads_.payload2_.getSize(), boost::asio::buffer_size(sendBuffer_));
    EXPECT_EQ(memcmp(&samplePayloads_.payload2_[0],boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), samplePayloads_.payload2_.getSize()), 0);
}

TEST_F(TcpConnectionTest, PendingBuffersShouldBeSendTogether)
{
    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i)
    {
        boost::asio::mutable_buffer sendBuffer1;
        asio::io_service::IoHandler sendHandler1;
        EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer1), SaveArg<1>(&sendHandler1)));

        connection_.send(samplePayloads_.payload1_);

        ASSERT_EQ(samplePayloads_.payload1_.getSize(), boost::asio::buffer_size(sendBuffer1));
        EXPECT_EQ(memcmp(&samplePayloads_.payload1_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.payload1_.getSize()), 0);

        connection_.send(samplePayloads_.halfFilledPayload_);
        connection_.send(samplePayloads_.halfFilledPayload_);

        boost::asio::mutable_buffer sendBuffer2;
        asio::io_service::IoHandler sendHandler2;
        EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer2), SaveArg<1>(&sendHandler2)));

        sendHandler1(boost::system::error_code(), samplePayloads_.payload1_.getSize());

        ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), boost::asio::buffer_size(sendBuffer2));
        EXPECT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer2), samplePayloads_.fullFilledPayload_.getSize()), 0);

        sendHandler2(boost::system::error_code(), samplePayloads_.fullFilledPayload_.getSize());
    }
}

TEST_F(TcpConnectionTest, sendErrorShouldTriggerCloseEvent)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(samplePayloads_.payload3_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    sendHandler_(boost::asio::error::bad_descriptor, 0);
}

TEST_F(TcpConnectionTest, sendErrorShouldNotTiggerCloseEventWhenSocketIsNotOpen)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(samplePayloads_.payload1_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    sendHandler_(boost::asio::error::already_started, 0);
}

TEST_F(TcpConnectionTest, overflowSecondarySendBufferShouldTriggerCloseEvent)
{
    EXPECT_CALL(*socket_, async_send(_, _));

    Payload payload1(samplePayloads_.payload1_);
    connection_.send(payload1);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    connection_.send(samplePayloads_.fullFilledPayload_);
    connection_.send(samplePayloads_.halfFilledPayload_);
}

TEST_F(TcpConnectionTest, sendWithOperationAbortedErrorShouldNotTriggerCloseEvent)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(samplePayloads_.payload2_);

    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    sendHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, connectShouldReturnFalseWhenErrorOccuredAndCloseSocketIfOpened)
{
    EXPECT_CALL(*socket_, connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::asio::error::access_denied));

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(*socket_, close());
    EXPECT_CALL(*socket_, shutdown(boost::asio::ip::tcp::socket::shutdown_both));

    ASSERT_FALSE(connection_.connect(endpoint_));
}

TEST_F(TcpConnectionTest, connectShouldReturnFalseWhenErrorOccuredAndDoNotCloseSocketIfNotOpened)
{
    EXPECT_CALL(*socket_, connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::asio::error::access_denied));

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));

    ASSERT_FALSE(connection_.connect(endpoint_));
}

TEST_F(TcpConnectionTest, connectShouldReturnTrueWhenNoErrorOccured)
{
    EXPECT_CALL(*socket_, connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::system::error_code()));
    EXPECT_CALL(*socket_, async_receive(_, _));
    ASSERT_TRUE(connection_.connect(endpoint_));
}

TEST_F(TcpConnectionTest, thrownExceptionFromShutdownDuringCloseOperationShouldBeCaught)
{
    EXPECT_CALL(*socket_, shutdown(boost::asio::ip::tcp::socket::shutdown_both)).WillOnce(Throw(boost::system::system_error(boost::asio::error::bad_descriptor)));

    connection_.close();
}


TEST_F(TcpConnectionTest, thrownExceptionFromCloseDuringCloseOperationShouldBeCaught)
{
    EXPECT_CALL(*socket_, shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(*socket_, close()).WillOnce(Throw(boost::system::system_error(boost::asio::error::fd_set_failure)));

    connection_.close();
}

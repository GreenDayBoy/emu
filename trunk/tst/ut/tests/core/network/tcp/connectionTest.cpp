#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>
#include <core/network/tcp/protocol.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

using ::testing::SaveArg;
using ::testing::SetArgReferee;
using ::testing::Return;
using ::testing::_;
using ::testing::Throw;

using eMU::ut::env::core::network::SamplePayloads;

using eMU::core::network::tcp::Protocol;
using eMU::core::network::tcp::Connection;
using eMU::core::network::Payload;

namespace asioStub = eMU::ut::env::asioStub;

class TcpConnectionTest: public ::testing::Test
{
protected:
    class ProtocolMock: public Protocol
    {
    public:
        MOCK_METHOD1(attach, bool(Connection::Pointer connection));
        MOCK_METHOD1(detach, void(Connection::Pointer connection));
        MOCK_METHOD1(dispatch, bool(Connection::Pointer connection));
    };

    TcpConnectionTest():
        connection_(new Connection(ioService_, protocol_)),
        endpoint_(boost::asio::ip::tcp::v4(), 55962) {}

    asioStub::io_service ioService_;
    ProtocolMock protocol_;
    Connection::Pointer connection_;
    boost::asio::ip::tcp::endpoint endpoint_;

    boost::asio::mutable_buffer receiveBuffer_;
    boost::asio::mutable_buffer sendBuffer_;

    SamplePayloads samplePayloads_;
};

TEST_F(TcpConnectionTest, close)
{
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(connection_->getSocket(), close());

    connection_->close();
}

TEST_F(TcpConnectionTest, disconnectShouldTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->disconnect();
}

TEST_F(TcpConnectionTest, disconnectAtNotOpenedSocketShouldNotTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(false));

    connection_->disconnect();
}

TEST_F(TcpConnectionTest, receiveWithEofErrorShouldTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    connection_->queueReceive();

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->receiveHandler(boost::asio::error::eof, 0);
}

TEST_F(TcpConnectionTest, receive)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _)).WillOnce(SaveArg<0>(&receiveBuffer_));

    connection_->queueReceive();

    ASSERT_TRUE(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_) != nullptr);
    ASSERT_EQ(Payload::getMaxSize(), boost::asio::buffer_size(receiveBuffer_));

    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &samplePayloads_.payload1_[0], samplePayloads_.payload1_.getSize());

    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));
    EXPECT_CALL(protocol_, dispatch(connection_)).WillOnce(Return(true));

    connection_->receiveHandler(boost::system::error_code(), samplePayloads_.payload1_.getSize());

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), connection_->getReadPayload().getSize());
    EXPECT_EQ(memcmp(&samplePayloads_.payload1_[0], &connection_->getReadPayload()[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(TcpConnectionTest, receiveErrorShouldTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    connection_->queueReceive();

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->receiveHandler(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, whenDispatchFailedThenCloseEventShouldBeTrigger)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    connection_->queueReceive();

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, dispatch(connection_)).WillOnce((Return(false)));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->receiveHandler(boost::system::error_code(), samplePayloads_.payload1_.getSize());
}

TEST_F(TcpConnectionTest, receiveErrorShouldNotTriggerCloseEventWhenSocketIsNotOpen)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    connection_->queueReceive();

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(false));

    connection_->receiveHandler(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, receiveWithOperationAbortedErrorShouldNotTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    connection_->queueReceive();

    connection_->receiveHandler(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, send)
{
    EXPECT_CALL(connection_->getSocket(), async_send(_, _)).WillOnce(SaveArg<0>(&sendBuffer_));

    connection_->send(samplePayloads_.payload2_);

    ASSERT_EQ(samplePayloads_.payload2_.getSize(), boost::asio::buffer_size(sendBuffer_));
    EXPECT_EQ(memcmp(&samplePayloads_.payload2_[0],boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), samplePayloads_.payload2_.getSize()), 0);
}

TEST_F(TcpConnectionTest, PendingBuffersShouldBeSendTogether)
{
    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i)
    {
        boost::asio::mutable_buffer sendBuffer1;
        EXPECT_CALL(connection_->getSocket(), async_send(_, _)).WillOnce(SaveArg<0>(&sendBuffer1));
        connection_->send(samplePayloads_.payload1_);

        ASSERT_EQ(samplePayloads_.payload1_.getSize(), boost::asio::buffer_size(sendBuffer1));
        EXPECT_EQ(memcmp(&samplePayloads_.payload1_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.payload1_.getSize()), 0);

        connection_->send(samplePayloads_.halfFilledPayload_);
        connection_->send(samplePayloads_.halfFilledPayload_);

        boost::asio::mutable_buffer sendBuffer2;
        EXPECT_CALL(connection_->getSocket(), async_send(_, _)).WillOnce(SaveArg<0>(&sendBuffer2));

        connection_->sendHandler(boost::system::error_code(), samplePayloads_.payload1_.getSize());

        ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), boost::asio::buffer_size(sendBuffer2));
        EXPECT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer2), samplePayloads_.fullFilledPayload_.getSize()), 0);

        connection_->sendHandler(boost::system::error_code(), samplePayloads_.fullFilledPayload_.getSize());
    }
}

TEST_F(TcpConnectionTest, sendErrorShouldTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_send(_, _));
    connection_->send(samplePayloads_.payload3_);

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->sendHandler(boost::asio::error::bad_descriptor, 0);
}

TEST_F(TcpConnectionTest, sendErrorShouldNotTiggerCloseEventWhenSocketIsNotOpen)
{
    EXPECT_CALL(connection_->getSocket(), async_send(_, _));
    connection_->send(samplePayloads_.payload1_);

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(false));

    connection_->sendHandler(boost::asio::error::already_started, 0);
}

TEST_F(TcpConnectionTest, overflowSecondarySendBufferShouldTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_send(_, _));

    Payload payload1(samplePayloads_.payload1_);
    connection_->send(payload1);

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->send(samplePayloads_.fullFilledPayload_);
    connection_->send(samplePayloads_.halfFilledPayload_);
}

TEST_F(TcpConnectionTest, sendWithOperationAbortedErrorShouldNotTriggerCloseEvent)
{
    EXPECT_CALL(connection_->getSocket(), async_send(_, _));
    connection_->send(samplePayloads_.payload2_);

    connection_->sendHandler(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, connectShouldReturnFalseWhenErrorOccuredAndCloseSocketIfOpened)
{
    EXPECT_CALL(connection_->getSocket(), connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::asio::error::access_denied));

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connection_->getSocket(), close());
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));

    ASSERT_FALSE(connection_->connect(endpoint_));
}

TEST_F(TcpConnectionTest, connectShouldReturnFalseWhenErrorOccuredAndDoNotCloseSocketIfNotOpened)
{
    EXPECT_CALL(connection_->getSocket(), connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::asio::error::access_denied));

    EXPECT_CALL(connection_->getSocket(), is_open()).WillOnce(Return(false));

    ASSERT_FALSE(connection_->connect(endpoint_));
}

TEST_F(TcpConnectionTest, connectShouldReturnFalseWhenAttachFailed)
{
    EXPECT_CALL(protocol_, attach(connection_)).WillOnce(Return(false));
    EXPECT_CALL(connection_->getSocket(), close());
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(connection_->getSocket(), connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::system::error_code()));

    ASSERT_FALSE(connection_->connect(endpoint_));
}

TEST_F(TcpConnectionTest, connectShouldReturnTrueWhenNoErrorOccured)
{
    EXPECT_CALL(protocol_, attach(connection_)).WillOnce(Return(true));
    EXPECT_CALL(connection_->getSocket(), connect(endpoint_, _)).WillOnce(SetArgReferee<1>(boost::system::error_code()));
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));

    ASSERT_TRUE(connection_->connect(endpoint_));
}

TEST_F(TcpConnectionTest, thrownExceptionFromShutdownDuringCloseOperationShouldBeCaught)
{
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both)).WillOnce(Throw(boost::system::system_error(boost::asio::error::bad_descriptor)));

    connection_->close();
}


TEST_F(TcpConnectionTest, thrownExceptionFromCloseDuringCloseOperationShouldBeCaught)
{
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(connection_->getSocket(), close()).WillOnce(Throw(boost::system::system_error(boost::asio::error::fd_set_failure)));

    connection_->close();
}

TEST_F(TcpConnectionTest, acceptShouldQueueReceiveWhenAttachIsSucceed)
{
    EXPECT_CALL(connection_->getSocket(), async_receive(_, _));
    EXPECT_CALL(protocol_, attach(connection_)).WillOnce(Return(true));

    connection_->accept();
}

TEST_F(TcpConnectionTest, acceptShouldCloseConnectionWhenAttachIsFailed)
{
    EXPECT_CALL(connection_->getSocket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(connection_->getSocket(), close()).WillOnce(Throw(boost::system::system_error(boost::asio::error::fd_set_failure)));
    EXPECT_CALL(protocol_, attach(connection_)).WillOnce(Return(false));

    connection_->accept();
}


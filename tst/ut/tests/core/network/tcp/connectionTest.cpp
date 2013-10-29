#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>
#include <ut/env/core/network/tcp/connectionEventsMock.hpp>

using ::testing::SaveArg;
using ::testing::Return;
using ::testing::Ref;
using ::testing::_;
using ::testing::Throw;

namespace asioStub = eMU::ut::env::asioStub;
namespace network = eMU::core::network;
namespace tcpEnv = eMU::ut::env::core::tcp;

class TcpConnectionTest: public ::testing::Test
{
public:
    TcpConnectionTest():
        socket_(new asioStub::ip::tcp::socket(ioService_)),
        connection_(socket_),
        defaultPayload_(100, 0x10),
        endpoint_(boost::asio::ip::tcp::v4(), 55962) {}

    void SetUp()
    {
        connection_.setConnectEventCallback(std::bind(&tcpEnv::ConnectionEventsMock::connectEvent, &connectionEvents_, std::placeholders::_1));
        connection_.setReceiveEventCallback(std::bind(&tcpEnv::ConnectionEventsMock::receiveEvent, &connectionEvents_, std::placeholders::_1));
        connection_.setCloseEventCallback(std::bind(&tcpEnv::ConnectionEventsMock::closeEvent, &connectionEvents_, std::placeholders::_1));
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

    void expectAsyncConnectCallAndSaveArguments()
    {
        EXPECT_CALL(*socket_, async_connect(endpoint_, _)).WillOnce(SaveArg<1>(&connectHandler_));
    }

    void insertFakePayload(const network::Payload &payload, boost::asio::mutable_buffer &buffer)
    {
        memcpy(boost::asio::buffer_cast<uint8_t*>(buffer), &payload[0], payload.size());
    }

    bool isPayloadTheSame(const network::Payload &payload, const uint8_t *sourceBuffer)
    {
        return memcmp(&payload[0], sourceBuffer, payload.size()) == 0;
    }  

    asioStub::io_service ioService_;
    network::tcp::Connection::SocketPointer socket_;
    network::tcp::Connection connection_;
    network::Payload defaultPayload_;
    boost::asio::ip::tcp::endpoint endpoint_;

    eMU::ut::env::core::tcp::ConnectionEventsMock connectionEvents_;

    asioStub::io_service::IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;

    asioStub::io_service::IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;

    asioStub::ip::tcp::socket::ConnectHandler connectHandler_;
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
    ASSERT_EQ(network::kMaxPayloadSize, boost::asio::buffer_size(receiveBuffer_));

    network::Payload payload(100, 0x14);
    insertFakePayload(payload, receiveBuffer_);

    EXPECT_CALL(*socket_, async_receive(_, _));
    EXPECT_CALL(connectionEvents_, receiveEvent(Ref(connection_)));

    receiveHandler_(boost::system::error_code(), payload.size());

    ASSERT_EQ(payload.size(), connection_.readBuffer().payloadSize_);
    EXPECT_TRUE(isPayloadTheSame(payload, &connection_.readBuffer().payload_[0]));
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

TEST_F(TcpConnectionTest, receiveWithConnectionResetErrorShouldNotTriggerCloseEvent)
{
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    receiveHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(TcpConnectionTest, send)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    ASSERT_EQ(defaultPayload_.size(), boost::asio::buffer_size(sendBuffer_));
    EXPECT_TRUE(isPayloadTheSame(defaultPayload_, boost::asio::buffer_cast<const uint8_t*>(sendBuffer_)));
}

TEST_F(TcpConnectionTest, PendingBuffersShouldBeSendTogether)
{
    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i)
    {
        boost::asio::mutable_buffer sendBuffer1;
        asioStub::io_service::IoHandler sendHandler1;
        EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer1), SaveArg<1>(&sendHandler1)));

        eMU::core::network::Payload payload1(100, 0x10);
        connection_.send(payload1);

        ASSERT_EQ(payload1.size(), boost::asio::buffer_size(sendBuffer1));
        EXPECT_TRUE(isPayloadTheSame(payload1, boost::asio::buffer_cast<const uint8_t*>(sendBuffer1)));

        eMU::core::network::Payload payload2(100, 0x11);
        connection_.send(payload2);

        eMU::core::network::Payload payload3(100, 0x12);
        connection_.send(payload3);

        boost::asio::mutable_buffer sendBuffer2;
        eMU::ut::env::asioStub::io_service::IoHandler sendHandler2;
        EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer2), SaveArg<1>(&sendHandler2)));

        sendHandler1(boost::system::error_code(), payload1.size());

        payload2.insert(payload2.end(), payload3.begin(), payload3.end());

        ASSERT_EQ(payload2.size(), boost::asio::buffer_size(sendBuffer2));
        EXPECT_TRUE(isPayloadTheSame(payload2, boost::asio::buffer_cast<const uint8_t*>(sendBuffer2)));

        sendHandler2(boost::system::error_code(), payload2.size());
    }
}

TEST_F(TcpConnectionTest, sendErrorShouldTriggerCloseEvent)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    sendHandler_(boost::asio::error::bad_descriptor, 0);
}

TEST_F(TcpConnectionTest, sendErrorShouldNotTiggerCloseEventWhenSocketIsNotOpen)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    sendHandler_(boost::asio::error::already_started, 0);
}

TEST_F(TcpConnectionTest, overflowPrimarySendBufferShouldTriggerCloseEvent)
{
    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    eMU::core::network::Payload payload(eMU::core::network::kMaxPayloadSize + 1, 0x10);
    connection_.send(payload);
}

TEST_F(TcpConnectionTest, overflowSecondarySendBufferShouldTriggerCloseEvent)
{
    EXPECT_CALL(*socket_, async_send(_, _));

    eMU::core::network::Payload payload1(eMU::core::network::kMaxPayloadSize, 0x10);
    connection_.send(payload1);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    eMU::core::network::Payload payload2(eMU::core::network::kMaxPayloadSize + 1, 0x11);
    connection_.send(payload2);
}

TEST_F(TcpConnectionTest, sendWithOperationAbortedErrorShouldNotTriggerCloseEvent)
{
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    sendHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, connect)
{
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(connectionEvents_, connectEvent(Ref(connection_)));
    connectHandler_(boost::system::error_code());
}

TEST_F(TcpConnectionTest, connectErrorShouldTriggerCloseEvent)
{
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    connectHandler_(boost::asio::error::access_denied);
}

TEST_F(TcpConnectionTest, connectErrorShouldNotTriggerCloseEventWhenSocketIsNotOpen)
{
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    connectHandler_(boost::asio::error::access_denied);
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

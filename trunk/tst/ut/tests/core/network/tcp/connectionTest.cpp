#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>
#include <ut/env/core/network/tcp/connectionEventsMock.hpp>

using ::testing::SaveArg;
using ::testing::Return;
using ::testing::Ref;
using ::testing::_;
using ::testing::NotNull;

class TcpConnectionTest: public ::testing::Test {
public:
    TcpConnectionTest():
      connection_(ioService_),
      defaultPayload_(100, 0x10),
      endpoint_(boost::asio::ip::tcp::v4(), 55962) {}

    void SetUp() {
        connection_.setConnectEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionEventsMock::connectEvent, &connectionEvents_, std::placeholders::_1));
        connection_.setReceiveEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionEventsMock::receiveEvent, &connectionEvents_, std::placeholders::_1));
        connection_.setCloseEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionEventsMock::closeEvent, &connectionEvents_, std::placeholders::_1));
    }
    
    void TearDown() {}

    void expectAsyncReceiveCallAndSaveArguments() {
        EXPECT_CALL(connection_.socket(), async_receive(_, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_), SaveArg<1>(&receiveHandler_)));
    }

    void expectAsyncSendCallAndSaveArguments() {
        EXPECT_CALL(connection_.socket(), async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer_), SaveArg<1>(&sendHandler_)));
    }

    void expectAsyncConnectCallAndSaveArguments() {
        EXPECT_CALL(connection_.socket(), async_connect(endpoint_, _)).WillOnce(SaveArg<1>(&connectHandler_));
    }

    eMU::core::network::tcp::Connection connection_;
    eMU::ut::env::asioStub::io_service ioService_;
    eMU::ut::env::core::tcp::ConnectionEventsMock connectionEvents_;
    eMU::ut::env::asioStub::io_service::IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;
    eMU::ut::env::asioStub::io_service::IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;
    eMU::core::network::Payload defaultPayload_;
    eMU::ut::env::asioStub::ip::tcp::socket::ConnectHandler connectHandler_;
    boost::asio::ip::tcp::endpoint endpoint_;
};

TEST_F(TcpConnectionTest, close) {
    EXPECT_CALL(connection_.socket(), shutdown(boost::asio::ip::tcp::socket::shutdown_both));
    EXPECT_CALL(connection_.socket(), close());

    connection_.close();
}

TEST_F(TcpConnectionTest, disconnect) {
    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    connection_.disconnect();
}

TEST_F(TcpConnectionTest, disconnect_notOpenedSocketShouldBeIgnored)
{
    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    connection_.disconnect();
}

TEST_F(TcpConnectionTest, disconnect_remote) {
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(TcpConnectionTest, receive) {
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    ASSERT_THAT(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_), NotNull());
    ASSERT_EQ(boost::asio::buffer_size(receiveBuffer_), eMU::core::network::kMaxPayloadSize);

    eMU::core::network::Payload payload(100, 0x14);
    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &payload[0], payload.size());

    EXPECT_CALL(connection_.socket(), async_receive(_, _));
    EXPECT_CALL(connectionEvents_, receiveEvent(Ref(connection_)));

    receiveHandler_(boost::system::error_code(), payload.size());

    ASSERT_EQ(payload.size(), connection_.readBuffer().payloadSize_);
    int result = memcmp(&payload[0], &connection_.readBuffer().payload_[0], payload.size());
    EXPECT_EQ(0, result);
}

TEST_F(TcpConnectionTest, receive_error) {
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));
    receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, receive_error_closeEventShouldNotBeCalledWhenSocketIsNotOpen) {
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);
    receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(TcpConnectionTest, receive_whenOperationWasAbortedCloseEventCallbackShouldNotBeCalled) {
    expectAsyncReceiveCallAndSaveArguments();

    connection_.queueReceive();

    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);
    receiveHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, send) {
    boost::asio::mutable_buffer sendBuffer;
    EXPECT_CALL(connection_.socket(), async_send(_, _)).WillOnce(SaveArg<0>(&sendBuffer));

    connection_.send(defaultPayload_);

    ASSERT_EQ(defaultPayload_.size(), boost::asio::buffer_size(sendBuffer));
    int32_t result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer), &defaultPayload_[0], defaultPayload_.size());
    ASSERT_EQ(0, result);
}

TEST_F(TcpConnectionTest, send_pending) {
    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        boost::asio::mutable_buffer buffer1;
        eMU::ut::env::asioStub::io_service::IoHandler sendHandler1;
        EXPECT_CALL(connection_.socket(), async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&buffer1), SaveArg<1>(&sendHandler1)));

        eMU::core::network::Payload payload1(100, 0x10);
        connection_.send(payload1);

        ASSERT_EQ(payload1.size(), boost::asio::buffer_size(buffer1));
        int32_t result = memcmp(boost::asio::buffer_cast<const uint8_t*>(buffer1), &payload1[0], payload1.size());
        ASSERT_EQ(0, result);

        eMU::core::network::Payload payload2(100, 0x11);
        connection_.send(payload2);

        eMU::core::network::Payload payload3(100, 0x12);
        connection_.send(payload3);

        boost::asio::mutable_buffer buffer2;
        eMU::ut::env::asioStub::io_service::IoHandler sendHandler2;
        EXPECT_CALL(connection_.socket(), async_send(_, _)).WillOnce(DoAll(SaveArg<0>(&buffer2), SaveArg<1>(&sendHandler2)));

        sendHandler1(boost::system::error_code(), payload1.size());

        eMU::core::network::Payload payload2And3;
        payload2And3.insert(payload2And3.end(), payload2.begin(), payload2.end());
        payload2And3.insert(payload2And3.end(), payload3.begin(), payload3.end());

        ASSERT_EQ(payload2And3.size(), boost::asio::buffer_size(buffer2));
        result = memcmp(boost::asio::buffer_cast<const uint8_t*>(buffer2), &payload2And3[0], payload2And3.size());
        ASSERT_EQ(0, result);

        sendHandler2(boost::system::error_code(), payload2And3.size());
    }
}

TEST_F(TcpConnectionTest, send_error) {
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    sendHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(TcpConnectionTest, send_error_closeEventShouldNotBeCalledWhenSocketIsNotOpen) {
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    sendHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(TcpConnectionTest, send__overflow_primary_buffer) {
    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    eMU::core::network::Payload payload(eMU::core::network::kMaxPayloadSize + 1, 0x10);
    connection_.send(payload);
}

TEST_F(TcpConnectionTest, send__overflow_secondary_buffer) {
    EXPECT_CALL(connection_.socket(), async_send(_, _));

    eMU::core::network::Payload payload1(eMU::core::network::kMaxPayloadSize, 0x10);
    connection_.send(payload1);

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    eMU::core::network::Payload payload2(eMU::core::network::kMaxPayloadSize + 1, 0x11);
    connection_.send(payload2);
}

TEST_F(TcpConnectionTest, send__operation_aborted) {
    expectAsyncSendCallAndSaveArguments();

    connection_.send(defaultPayload_);

    sendHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(TcpConnectionTest, connect) {
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(connectionEvents_, connectEvent(Ref(connection_)));
    connectHandler_(boost::system::error_code());
}

TEST_F(TcpConnectionTest, connect_error) {
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionEvents_, closeEvent(Ref(connection_)));

    connectHandler_(boost::asio::error::access_denied);
}

TEST_F(TcpConnectionTest, connect_error_closeEventShouldNotBeCalledWhenSocketIsNotOpen) {
    expectAsyncConnectCallAndSaveArguments();

    connection_.connect(endpoint_);

    EXPECT_CALL(connection_.socket(), is_open()).WillOnce(Return(false));
    EXPECT_CALL(connectionEvents_, closeEvent(_)).Times(0);

    connectHandler_(boost::asio::error::access_denied);
}

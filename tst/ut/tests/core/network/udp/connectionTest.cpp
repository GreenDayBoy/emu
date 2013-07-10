#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/udp/connection.hpp>
#include <ut/env/core/network/udp/connectionEventsMock.hpp>

using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::_;
using ::testing::NotNull;
using ::testing::ActionInterface;

ACTION_TEMPLATE(SaveArgToPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer)) {
  *pointer = &(::std::tr1::get<k>(args));
}

class UdpConnectionTest: public ::testing::Test {
public:
    UdpConnectionTest():
      connection_(ioService_, 12345) {}

    void SetUp() {
        connection_.setReceiveFromEventCallback(std::bind(&eMU::ut::env::core::udp::ConnectionEventsMock::receiveFromEvent,
                                                          &connectionEvents_,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2));
    }
    
    void TearDown() {}

    void expectAsyncReceiveFromCallAndSaveArguments() {
        EXPECT_CALL(connection_.socket(), async_receive_from(_, _, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_),
                                                                                      SaveArgToPointer<1>(&senderEndpoint_),
                                                                                      SaveArg<2>(&receiveFromHandler_)));
    }

    void expectAsyncSendToCallAndSaveArguments() {
        EXPECT_CALL(connection_.socket(), async_send_to(_, _, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer_),
                                                                                 SaveArg<1>(&receiverEndpoint_),
                                                                                 SaveArg<2>(&sendToHandler_)));
    }

    eMU::core::network::udp::Connection connection_;
    eMU::ut::env::asioStub::io_service ioService_;
    eMU::ut::env::core::udp::ConnectionEventsMock connectionEvents_;

    boost::asio::mutable_buffer receiveBuffer_;
    eMU::ut::env::asioStub::io_service::IoHandler receiveFromHandler_;
    boost::asio::ip::udp::endpoint* senderEndpoint_;

    boost::asio::mutable_buffer sendBuffer_;
    eMU::ut::env::asioStub::io_service::IoHandler sendToHandler_;
    boost::asio::ip::udp::endpoint receiverEndpoint_;
};

TEST_F(UdpConnectionTest, receiveFrom) {
    expectAsyncReceiveFromCallAndSaveArguments();
    connection_.queueReceiveFrom();

    ASSERT_THAT(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_), NotNull());
    ASSERT_EQ(boost::asio::buffer_size(receiveBuffer_), eMU::core::network::kMaxPayloadSize);

    // Insert prepared payload to connection rbuf.
    eMU::core::network::Payload payload(100, 0x14);
    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &payload[0], payload.size());

    // Prepare fake sender endpoint.
    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    // Set fake endpoint.
    *senderEndpoint_ = senderEndpoint;

    EXPECT_CALL(connection_.socket(), async_receive_from(_, _, _));
    EXPECT_CALL(connectionEvents_, receiveFromEvent(::testing::Ref(connection_), senderEndpoint));
    receiveFromHandler_(boost::system::error_code(), payload.size());

    // Check if we received exactly prepared payload.
    ASSERT_EQ(payload.size(), connection_.readBuffer().payloadSize_);
    int result = memcmp(&payload[0], &connection_.readBuffer().payload_[0], payload.size());
    EXPECT_EQ(0, result);
}

TEST_F(UdpConnectionTest, receiveFrom_error) {
    expectAsyncReceiveFromCallAndSaveArguments();
    connection_.queueReceiveFrom();

    EXPECT_CALL(connection_.socket(), async_receive_from(_, _, _));
    receiveFromHandler_(boost::asio::error::fault, 0);
}

TEST_F(UdpConnectionTest, sendTo) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::Payload payload(100, 0x14);

    expectAsyncSendToCallAndSaveArguments();
    connection_.sendTo(endpoint, payload);

    EXPECT_EQ(receiverEndpoint_, endpoint);

    // Check if payload set to socket is the same as payload requested to send.
    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), payload.size());

    int32_t result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

TEST_F(UdpConnectionTest, sendTo_pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    eMU::core::network::Payload payload1(100, 0x10);
    eMU::core::network::Payload payload2(100, 0x11);
    eMU::core::network::Payload payload3(100, 0x12);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        boost::asio::mutable_buffer sendBuffer1;
        eMU::ut::env::asioStub::io_service::IoHandler sendToHandler1;

        EXPECT_CALL(connection_.socket(), async_send_to(_, _, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer1),
                                                                                 SaveArg<2>(&sendToHandler1)));

        // Queue send which should block primary buffer.
        connection_.sendTo(endpoint, payload1);

        // Check if first payload queued to send
        ASSERT_EQ(boost::asio::buffer_size(sendBuffer1), payload1.size());
        int result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), &payload1[0], payload1.size());
        ASSERT_EQ(0, result);

        // Payload from those two sends should be stored in secondary buffer.
        connection_.sendTo(endpoint, payload2);
        connection_.sendTo(endpoint, payload3);

        boost::asio::mutable_buffer sendBuffer2;
        eMU::ut::env::asioStub::io_service::IoHandler sendToHandler2;

        EXPECT_CALL(connection_.socket(), async_send_to(_, _, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer2),
                                                                                 SaveArg<2>(&sendToHandler2)));
        // Dequeue this send should release primary buffer.
        // Now pending send operations can be handled as one operation.
        sendToHandler1(boost::system::error_code(), payload1.size());

        eMU::core::network::Payload payload2And3;
        payload2And3.insert(payload2And3.end(), payload2.begin(), payload2.end());
        payload2And3.insert(payload2And3.end(), payload3.begin(), payload3.end());

        // Check if second and third payloads queued to send
        ASSERT_EQ(boost::asio::buffer_size(sendBuffer2), payload2And3.size());
        result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer2), &payload2And3[0], payload2And3.size());
        ASSERT_EQ(0, result);

        sendToHandler2(boost::system::error_code(), payload2And3.size());
    }
}

TEST_F(UdpConnectionTest, sendTo_error) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::Payload payload(100, 0x10);

    expectAsyncSendToCallAndSaveArguments();
    connection_.sendTo(endpoint, payload);

    sendToHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(UdpConnectionTest, sendTo_error_with_pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::Payload payload(100, 0x10);
    eMU::core::network::Payload payload2(100, 0x11);

    expectAsyncSendToCallAndSaveArguments();
    connection_.sendTo(endpoint, payload);

    // Payload from this sendTo should be stored in secondary buffer.
    connection_.sendTo(endpoint, payload2);

    // Dequeue with error primary payload but pending payload should be queued normally.
    expectAsyncSendToCallAndSaveArguments();
    sendToHandler_(boost::asio::error::connection_reset, 0);

    // Check if payload set to socket is the same as pending payload.
    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), payload2.size());
    int32_t result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), &payload2[0], payload2.size());
    ASSERT_EQ(0, result);

    sendToHandler_(boost::system::error_code(), payload2.size());
}

TEST_F(UdpConnectionTest, sendTo_different_endpoints) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::Payload payload(100, 0x10);
    eMU::core::network::Payload payload2(100, 0x10);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    eMU::core::network::Payload payload3(100, 0x12);

    EXPECT_CALL(connection_.socket(), async_send_to(::testing::_, ::testing::_, ::testing::_));
    connection_.sendTo(endpoint, payload);

    // This paylaod should be pending because we are already sending
    // something to this endpoint.
    connection_.sendTo(endpoint, payload2);

    // Send some data to another endpoint.
    // Operation should be queued immediately.
    EXPECT_CALL(connection_.socket(), async_send_to(::testing::_, ::testing::_, ::testing::_));
    connection_.sendTo(endpoint2, payload3);
}

TEST_F(UdpConnectionTest, sendTo_overflow_buffer) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::Payload payload(eMU::core::network::kMaxPayloadSize + 1, 0x10);

    EXPECT_CALL(connection_.socket(), async_send_to(_, _, _)).Times(0);
    connection_.sendTo(endpoint, payload);
}

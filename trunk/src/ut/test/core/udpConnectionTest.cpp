#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/bind.hpp>

#include <env/core/udp/udpSocketMock.hpp>
#include <env/core/udp/udpConnectionEventCallbacksMock.hpp>
#include <env/core/ioServiceStub.hpp>

class udpConnectionTest_t: public ::testing::Test {
public:
    udpConnectionTest_t():
      connection_(ioService_, 12345) {}

    void SetUp() {
        connection_.receiveFromEventCallback(boost::bind(&eMU::ut::env::core::udp::connectionEventCallbacksMock_t::receiveFromEvent,
                                                         &connectionEvents_,
                                                         _1,
                                                         _2));
    }
    
    void TearDown() {}

    eMU::core::network::udp::connection_t<> connection_;
    eMU::ut::env::core::ioServiceStub_t ioService_;
    eMU::ut::env::core::udp::connectionEventCallbacksMock_t connectionEvents_;
};

TEST_F(udpConnectionTest_t, receiveFrom) {
    connection_.socket().expectCall_async_receive_from();
    connection_.queueReceiveFrom();

    ASSERT_THAT(connection_.socket().rbuf_, testing::NotNull());
    ASSERT_EQ(connection_.socket().rbufSize_, eMU::core::network::maxPayloadSize_c);

    // Insert prepared payload to connection rbuf.
    eMU::core::network::payload_t payload(100, 0x14);
    memcpy(connection_.socket().rbuf_, &payload[0], payload.size());

    // Prepare fake sender endpoint.
    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    // Set fake endpoint.
    *connection_.socket().senderEndpoint_ = senderEndpoint;

    connection_.socket().expectCall_async_receive_from();
    connectionEvents_.expectCall_receiveFromEvent(connection_, senderEndpoint);
    connection_.socket().receiveFromHandler_(boost::system::error_code(), payload.size());

    // Check if we received exactly prepared payload.
    ASSERT_EQ(payload.size(), connection_.rbuf().payloadSize_);
    int result = memcmp(&payload[0], &connection_.rbuf().payload_[0], payload.size());
    EXPECT_EQ(0, result);
}

TEST_F(udpConnectionTest_t, receiveFrom__error) {
    connection_.socket().expectCall_async_receive_from();
    connection_.queueReceiveFrom();

    connection_.socket().expectCall_async_receive_from();
    connection_.socket().receiveFromHandler_(boost::asio::error::fault, 0);
}

TEST_F(udpConnectionTest_t, sendTo) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::payload_t payload(100, 0x14);

    connection_.socket().expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    EXPECT_EQ(*connection_.socket().endpoint_, endpoint);

    // Check if payload set to socket is the same as payload requested to send.
    ASSERT_EQ(payload.size(), connection_.socket().wbufSize_);
    int32 result = memcmp(connection_.socket().wbuf_, &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

TEST_F(udpConnectionTest_t, sendTo__pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::payload_t payload1(100, 0x10);
    eMU::core::network::payload_t payload2(100, 0x11);

    // make different payload part for second pending send operation.
    payload2.resize(200, 0x12);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        connection_.socket().expectCall_async_send_to();

        // Queue send which should block primary buffer.
        connection_.sendTo(endpoint, &payload1[0], payload1.size());

        // Payload from those two sends should be stored in secondary buffer.
        connection_.sendTo(endpoint, &payload2[0], 100);
        connection_.sendTo(endpoint, &payload2[100], 100);

        connection_.socket().expectCall_async_send_to();
        // Dequeue this send should release primary buffer.
        // Now pending send operations can be handled as one operation.
        connection_.socket().sendToHandler_(boost::system::error_code(), payload1.size());

        // Check if now in primary buffer are pending send payloads
        ASSERT_EQ(payload2.size(), connection_.socket().wbufSize_);
        int result = memcmp(connection_.socket().wbuf_, &payload2[0], payload2.size());
        ASSERT_EQ(0, result);

        // Dequeue pending sends.
        connection_.socket().sendToHandler_(boost::system::error_code(), payload1.size());
    }
}

TEST_F(udpConnectionTest_t, sendTo__error) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::payload_t payload(100, 0x10);

    connection_.socket().expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    connection_.socket().sendToHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(udpConnectionTest_t, sendTo__error_with_pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::payload_t payload(100, 0x10);
    eMU::core::network::payload_t payload2(100, 0x11);

    connection_.socket().expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    // Payload from this sendTo should be stored in secondary buffer.
    connection_.sendTo(endpoint, &payload2[0], payload2.size());

    // Dequeue with error primary payload but pending payload should be queued normally.
    connection_.socket().expectCall_async_send_to();
    connection_.socket().sendToHandler_(boost::asio::error::connection_reset, 0);

    // Check if payload set to socket is the same as pending payload.
    ASSERT_EQ(payload2.size(), connection_.socket().wbufSize_);
    int32 result = memcmp(connection_.socket().wbuf_, &payload2[0], payload2.size());
    ASSERT_EQ(0, result);   
}

TEST_F(udpConnectionTest_t, sendTo__dif_endpoints) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::payload_t payload(100, 0x10);
    eMU::core::network::payload_t payload2(100, 0x10);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    eMU::core::network::payload_t payload3(100, 0x12);

    connection_.socket().expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    // This paylaod should be pending because we are already sending
    // something to this endpoint.
    connection_.sendTo(endpoint, &payload2[0], payload2.size());

    // Send some data to another endpoint.
    // Operation should be queued immediately.
    connection_.socket().expectCall_async_send_to();
    connection_.sendTo(endpoint2, &payload3[0], payload3.size()); 
}
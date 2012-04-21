#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "udpSocketMock.hpp"
#include "ioServiceStub.hpp"
#include "udpConnectionEventCallbacksMock.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;

class udpConnectionTest_t: public ::testing::Test {
public:
    udpConnectionTest_t():
      connection_(ioServiceStub_, 12345),
      socketMock_(NULL) {}

    void SetUp() {
        connection_.receiveFromEventCallback(boost::bind(&eMUNetworkUT::udp::connectionEventCallbacksMock_t::receiveFromEvent,
                                                         &connectionEventCallbacks_,
                                                         _1,
                                                         _2,
                                                         _3));

        socketMock_ = &connection_.socket();
    }
    
    void TearDown() {

    }

    eMUNetworkUT::udp::testConnection_t connection_;
    eMUNetworkUT::ioServiceStub_t ioServiceStub_;
    eMUNetworkUT::udp::socketMock_t *socketMock_;
    eMUNetworkUT::udp::connectionEventCallbacksMock_t connectionEventCallbacks_;
};

TEST_F(udpConnectionTest_t, receiveFrom) {
    socketMock_->expectCall_async_receive_from();
    connection_.queueReceiveFrom();

    ASSERT_THAT(socketMock_->rbuf_, testing::NotNull());
    ASSERT_EQ(socketMock_->rbufSize_, eMUNetwork::maxPayloadSize_c);

    // Insert prepared payload to connection rbuf.
    eMUNetwork::payload_t payload(100, 0x14);
    memcpy(socketMock_->rbuf_, &payload[0], payload.size());

    // Prepare fake sender endpoint.
    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    // Set fake endpoint.
    *socketMock_->senderEndpoint_ = senderEndpoint;

    socketMock_->expectCall_async_receive_from();
    connectionEventCallbacks_.expectCall_receiveFromEvent(&connection_, senderEndpoint, payload);
    socketMock_->receiveFromHandler_(boost::system::error_code(), payload.size());
}

TEST_F(udpConnectionTest_t, receiveFrom__error) {
    socketMock_->expectCall_async_receive_from();
    connection_.queueReceiveFrom();

    socketMock_->expectCall_async_receive_from();
    socketMock_->receiveFromHandler_(boost::asio::error::fault, 0);
}

TEST_F(udpConnectionTest_t, sendTo) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMUNetwork::payload_t payload(100, 0x14);

    socketMock_->expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    EXPECT_EQ(*socketMock_->endpoint_, endpoint);

    // Check if payload set to socket is the same as payload requested to send.
    ASSERT_EQ(payload.size(), socketMock_->wbufSize_);
    int32 result = memcmp(socketMock_->wbuf_, &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

TEST_F(udpConnectionTest_t, sendTo__pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMUNetwork::payload_t payload1(100, 0x10);
    eMUNetwork::payload_t payload2(100, 0x11);

    // make different payload part for second pending send operation.
    payload2.resize(200, 0x12);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        socketMock_->expectCall_async_send_to();

        // Queue send which should block primary buffer.
        connection_.sendTo(endpoint, &payload1[0], payload1.size());

        // Payload from those two sends should be stored in secondary buffer.
        connection_.sendTo(endpoint, &payload2[0], 100);
        connection_.sendTo(endpoint, &payload2[100], 100);

        socketMock_->expectCall_async_send_to();
        // Dequeue this send should release primary buffer.
        // Now pending send operations can be handled as one operation.
        socketMock_->sendToHandler_(boost::system::error_code(), payload1.size());

        // Check if now in primary buffer are pending send payloads
        ASSERT_EQ(payload2.size(), socketMock_->wbufSize_);
        int result = memcmp(socketMock_->wbuf_, &payload2[0], payload2.size());
        ASSERT_EQ(0, result);

        // Dequeue pending sends.
        socketMock_->sendToHandler_(boost::system::error_code(), payload1.size());
    }
}

TEST_F(udpConnectionTest_t, sendTo__error) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMUNetwork::payload_t payload(100, 0x10);

    socketMock_->expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    socketMock_->sendToHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(udpConnectionTest_t, sendTo__error_with_pending) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMUNetwork::payload_t payload(100, 0x10);
    eMUNetwork::payload_t payload2(100, 0x11);

    socketMock_->expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    // Payload from this sendTo should be stored in secondary buffer.
    connection_.sendTo(endpoint, &payload2[0], payload2.size());

    // Dequeue with error primary payload but pending payload should be queued normally.
    socketMock_->expectCall_async_send_to();
    socketMock_->sendToHandler_(boost::asio::error::connection_reset, 0);

    // Check if payload set to socket is the same as pending payload.
    ASSERT_EQ(payload2.size(), socketMock_->wbufSize_);
    int32 result = memcmp(socketMock_->wbuf_, &payload2[0], payload2.size());
    ASSERT_EQ(0, result);   
}

TEST_F(udpConnectionTest_t, sendTo__dif_endpoints) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMUNetwork::payload_t payload(100, 0x10);
    eMUNetwork::payload_t payload2(100, 0x10);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    eMUNetwork::payload_t payload3(100, 0x12);

    socketMock_->expectCall_async_send_to();
    connection_.sendTo(endpoint, &payload[0], payload.size());

    // This paylaod should be pending because we are already sending
    // something to this endpoint.
    connection_.sendTo(endpoint, &payload2[0], payload2.size());

    // Send some data to another endpoint.
    // Operation should be queued immediately.
    socketMock_->expectCall_async_send_to();
    connection_.sendTo(endpoint2, &payload3[0], payload3.size()); 
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../shared/types.hpp"
#include "socketMock.hpp"
#include "ioServiceStub.hpp"
#include "connectionObserverMock.hpp"
#include "../../core/connection.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;

class connectionTest_t: public ::testing::Test {
public:
    connectionTest_t():
      connection_(ioServiceStub_, connectionObserverMock_) {}

    void SetUp() {
        socketMock_ = &connection_.socket();
    }
    
    void TearDown() {

    }

    eMUNetwork::connection_t<eMUNetworkUT::socketMock_t,
                             eMUNetworkUT::ioServiceStub_t> connection_;
    eMUNetworkUT::ioServiceStub_t ioServiceStub_;
    eMUNetworkUT::connectionObserverMock_t connectionObserverMock_;
    eMUNetworkUT::socketMock_t *socketMock_;
};

TEST_F(connectionTest_t, close) {
    socketMock_->expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socketMock_->expectCall_close();

    connection_.close();
}

TEST_F(connectionTest_t, disconnect) {
    connectionObserverMock_.expectCall_closeEvent(&connection_);
    connection_.disconnect();
}

TEST_F(connectionTest_t, disconnect__remote) {
    socketMock_->expectCall_async_receive();
    connection_.queueReceive();

    connectionObserverMock_.expectCall_closeEvent(&connection_);
    socketMock_->receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(connectionTest_t, receive) {
    socketMock_->expectCall_async_receive();

    connection_.queueReceive();

    ASSERT_THAT(socketMock_->rbuf_, testing::NotNull());
    ASSERT_EQ(socketMock_->rbufSize_, eMUNetwork::maxPayloadSize_c);

    // Insert prepared payload to connection rbuf.
    eMUNetwork::payload_t payload(100, 0x14);
    memcpy(socketMock_->rbuf_, &payload[0], payload.size());

    socketMock_->expectCall_async_receive();
    connectionObserverMock_.expectCall_receiveEvent(&connection_, payload);

    socketMock_->receiveHandler_(boost::system::error_code(), payload.size());
}

TEST_F(connectionTest_t, receive__error) {
    socketMock_->expectCall_async_receive();
    connection_.queueReceive();

    connectionObserverMock_.expectCall_closeEvent(&connection_);
    socketMock_->receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(connectionTest_t, receive__operation_aborted) {
    socketMock_->expectCall_async_receive();
    connection_.queueReceive();

    socketMock_->receiveHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(connectionTest_t, send) {
    eMUNetwork::payload_t payload(100, 0x10);

    socketMock_->expectCall_async_send();

    connection_.send(&payload[0], payload.size());

    // Check if payload set to socket is the same as payload requested to send.
    ASSERT_EQ(payload.size(), socketMock_->wbufSize_);
    int32 result = memcmp(socketMock_->wbuf_, &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

TEST_F(connectionTest_t, send__pending) {
    eMUNetwork::payload_t payload1(100, 0x10);
    eMUNetwork::payload_t payload2(100, 0x11);

    // make different payload part for second pending send operation.
    payload2.resize(200, 0x12);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        socketMock_->expectCall_async_send();

        // Queue send which should block primary buffer.
        connection_.send(&payload1[0], payload1.size());

        // Payload from those two sends should be stored in secondary buffer.
        connection_.send(&payload2[0], 100);
        connection_.send(&payload2[100], 100);

        socketMock_->expectCall_async_send();
        // Dequeue this send should release primary buffer.
        // Now pending send operations can be handled as one operation.
        socketMock_->sendHandler_(boost::system::error_code(), payload1.size());

        // Check if now in primary buffer are pending send payloads
        ASSERT_EQ(payload2.size(), socketMock_->wbufSize_);
        int result = memcmp(socketMock_->wbuf_, &payload2[0], payload2.size());
        ASSERT_EQ(0, result);

        // Dequeue pending sends.
        socketMock_->sendHandler_(boost::system::error_code(), payload1.size());
    }
}

TEST_F(connectionTest_t, send__error) {
    eMUNetwork::payload_t payload(100, 0x10);

    socketMock_->expectCall_async_send();
    connection_.send(&payload[0], payload.size());

    connectionObserverMock_.expectCall_closeEvent(&connection_);
    socketMock_->sendHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(connectionTest_t, send__overflow_primary_buffer) {
    eMUNetwork::payload_t payload(eMUNetwork::maxPayloadSize_c + 1, 0x10);

    connectionObserverMock_.expectCall_closeEvent(&connection_);
    connection_.send(&payload[0], payload.size());
}

TEST_F(connectionTest_t, send__overflow_secondary_buffer) {
    eMUNetwork::payload_t payload1(eMUNetwork::maxPayloadSize_c, 0x10);
    eMUNetwork::payload_t payload2(eMUNetwork::maxPayloadSize_c + 1, 0x11);
    
    socketMock_->expectCall_async_send();
    connection_.send(&payload1[0], payload1.size());

    connectionObserverMock_.expectCall_closeEvent(&connection_);
    connection_.send(&payload2[0], payload2.size());
}

TEST_F(connectionTest_t, send__operation_aborted) {
    eMUNetwork::payload_t payload(100, 0x10);

    socketMock_->expectCall_async_send();
    connection_.send(&payload[0], payload.size());

    socketMock_->sendHandler_(boost::asio::error::operation_aborted, 0);
}
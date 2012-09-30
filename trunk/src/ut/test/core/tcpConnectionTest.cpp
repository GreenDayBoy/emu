#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <env/core/tcp/tcpSocketMock.hpp>
#include <env/core/tcp/tcpConnectionEventCallbacksMock.hpp>
#include <env/core/ioServiceStub.hpp>
#include <core/network/tcp/tcpConnection.hpp>

class tcpConnectionTest_t: public ::testing::Test {
public:
    tcpConnectionTest_t():
      connection_(ioService_) {}

    void SetUp() {
        connection_.connectEventCallback(boost::bind(&eMU::ut::env::core::tcp::connectionEventCallbacksMock_t::connectEvent, &connectionEvents_, _1));
        connection_.receiveEventCallback(boost::bind(&eMU::ut::env::core::tcp::connectionEventCallbacksMock_t::receiveEvent, &connectionEvents_, _1));
        connection_.closeEventCallback(boost::bind(&eMU::ut::env::core::tcp::connectionEventCallbacksMock_t::closeEvent, &connectionEvents_, _1));
    }
    
    void TearDown() {}

    eMU::core::network::tcp::connection_t<> connection_;
    eMU::ut::env::core::ioServiceStub_t ioService_;
    eMU::ut::env::core::tcp::connectionEventCallbacksMock_t connectionEvents_;
};

TEST_F(tcpConnectionTest_t, close) {
    connection_.socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection_.socket().expectCall_close();

    connection_.close();
}

TEST_F(tcpConnectionTest_t, disconnect) {
    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.disconnect();
}

TEST_F(tcpConnectionTest_t, disconnect__remote) {
    connection_.socket().expectCall_async_receive();
    connection_.queueReceive();

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.socket().receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(tcpConnectionTest_t, receive) {
    connection_.socket().expectCall_async_receive();

    connection_.queueReceive();

    ASSERT_THAT(connection_.socket().rbuf_, testing::NotNull());
    ASSERT_EQ(connection_.socket().rbufSize_, eMU::core::network::maxPayloadSize_c);

    // Insert prepared payload to connection rbuf.
    eMU::core::network::payload_t payload(100, 0x14);
    memcpy(connection_.socket().rbuf_, &payload[0], payload.size());

    connection_.socket().expectCall_async_receive();
    connectionEvents_.expectCall_receiveEvent(connection_);
    connection_.socket().receiveHandler_(boost::system::error_code(), payload.size());

    // Check if we received exactly prepared payload.
    ASSERT_EQ(payload.size(), connection_.rbuf().payloadSize_);
    int result = memcmp(&payload[0], &connection_.rbuf().payload_[0], payload.size());
    EXPECT_EQ(0, result);
}

TEST_F(tcpConnectionTest_t, receive__error) {
    connection_.socket().expectCall_async_receive();
    connection_.queueReceive();

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.socket().receiveHandler_(boost::asio::error::broken_pipe, 0);
}

TEST_F(tcpConnectionTest_t, receive__operation_aborted) {
    connection_.socket().expectCall_async_receive();
    connection_.queueReceive();

    connection_.socket().receiveHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(tcpConnectionTest_t, send) {
    eMU::core::network::payload_t payload(100, 0x10);

    connection_.socket().expectCall_async_send();

    connection_.send(&payload[0], payload.size());

    // Check if payload set to socket is the same as payload requested to send.
    ASSERT_EQ(payload.size(), connection_.socket().wbufSize_);
    int32 result = memcmp(connection_.socket().wbuf_, &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

TEST_F(tcpConnectionTest_t, send__pending) {
    eMU::core::network::payload_t payload1(100, 0x10);
    eMU::core::network::payload_t payload2(100, 0x11);

    // make different payload part for second pending send operation.
    payload2.resize(200, 0x12);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i) {
        connection_.socket().expectCall_async_send();

        // Queue send which should block primary buffer.
        connection_.send(&payload1[0], payload1.size());

        // Payload from those two sends should be stored in secondary buffer.
        connection_.send(&payload2[0], 100);
        connection_.send(&payload2[100], 100);

        connection_.socket().expectCall_async_send();
        // Dequeue this send should release primary buffer.
        // Now pending send operations can be handled as one operation.
        connection_.socket().sendHandler_(boost::system::error_code(), payload1.size());

        // Check if now in primary buffer are pending send payloads
        ASSERT_EQ(payload2.size(), connection_.socket().wbufSize_);
        int result = memcmp(connection_.socket().wbuf_, &payload2[0], payload2.size());
        ASSERT_EQ(0, result);

        // Dequeue pending sends.
        connection_.socket().sendHandler_(boost::system::error_code(), payload1.size());
    }
}

TEST_F(tcpConnectionTest_t, send__error) {
    eMU::core::network::payload_t payload(100, 0x10);

    connection_.socket().expectCall_async_send();
    connection_.send(&payload[0], payload.size());

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.socket().sendHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(tcpConnectionTest_t, send__overflow_primary_buffer) {
    eMU::core::network::payload_t payload(eMU::core::network::maxPayloadSize_c + 1, 0x10);

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.send(&payload[0], payload.size());
}

TEST_F(tcpConnectionTest_t, send__overflow_secondary_buffer) {
    eMU::core::network::payload_t payload1(eMU::core::network::maxPayloadSize_c, 0x10);
    eMU::core::network::payload_t payload2(eMU::core::network::maxPayloadSize_c + 1, 0x11);
    
    connection_.socket().expectCall_async_send();
    connection_.send(&payload1[0], payload1.size());

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.send(&payload2[0], payload2.size());
}

TEST_F(tcpConnectionTest_t, send__operation_aborted) {
    eMU::core::network::payload_t payload(100, 0x10);

    connection_.socket().expectCall_async_send();
    connection_.send(&payload[0], payload.size());

    connection_.socket().sendHandler_(boost::asio::error::operation_aborted, 0);
}

TEST_F(tcpConnectionTest_t, connect) {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 55962);

    connection_.socket().expectCall_async_connect(endpoint);
    connection_.connect(endpoint);

    connectionEvents_.expectCall_connectEvent(connection_);
    connection_.socket().connectHandler_(boost::system::error_code());
}

TEST_F(tcpConnectionTest_t, connect__error) {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 55962);

    connection_.socket().expectCall_async_connect(endpoint);
    connection_.connect(endpoint);

    connectionEvents_.expectCall_closeEvent(connection_);
    connection_.socket().connectHandler_(boost::asio::error::access_denied);
}
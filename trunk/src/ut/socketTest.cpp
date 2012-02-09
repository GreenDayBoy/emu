#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../shared/types.hpp"
#include "socketStub.hpp"
#include "ioServiceMock.hpp"
#include "eventHandlerMock.hpp"
#include "../core/socket.hpp"

class socketTest_t: public ::testing::Test {
public:
    void SetUp() {
        this->initializePatternBuf();
    }

    void TearDown() {}

    void initializePatternBuf() {
        uint8 value = 0;
        for(size_t i = 0; i < eMUCore::network::maxPayloadSize_c; ++i)
            patternBuf_.payload_[i] = value++;
    }

    void expectCall_ioService_read() {
        EXPECT_CALL(ioService_, read(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    }

    void expectCall_ioService_write(size_t size) {
        EXPECT_CALL(ioService_, write(::testing::NotNull(), size, ::testing::_));
    }

    void expectCall_ioService_shutdownSocket() {
        EXPECT_CALL(ioService_, shutdownSocket(boost::asio::ip::tcp::socket::shutdown_both));
    }

    void expectCall_eventHandler_onClose() {
        EXPECT_CALL(eventHandler_, onClose());
    }

    void expectCall_eventHandler_onReceive(size_t size) {
        EXPECT_CALL(eventHandler_, onReceive(::testing::NotNull(), size));
    }

    eMUUnitTest::networkTest::ioServiceMock_t ioService_;
    eMUUnitTest::networkTest::eventHandlerMock_t eventHandler_;
    eMUCore::network::readBuffer_t patternBuf_;
};

TEST_F(socketTest_t, construct) {
    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);
}

TEST_F(socketTest_t, close) {
    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_eventHandler_onClose();
    this->expectCall_ioService_shutdownSocket();
    socket.close();
}

TEST_F(socketTest_t, send) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    ioService_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_c);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);
}

TEST_F(socketTest_t, send_Pending) {
    ioService_.delegateMocks();

    size_t testPayloadSize = eMUCore::network::maxPayloadSize_c / 2;

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    socket.send(&patternBuf_.payload_[0], testPayloadSize);
    socket.send(&patternBuf_.payload_[testPayloadSize], testPayloadSize);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    ioService_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_c);
}

TEST_F(socketTest_t, send_Error) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    this->expectCall_eventHandler_onClose();
    this->expectCall_ioService_shutdownSocket();
    ioService_.dequeueWrite(boost::asio::error::connection_reset, 0);
}

TEST_F(socketTest_t, send_Overflow) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_ioService_write(eMUCore::network::maxPayloadSize_c);
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    this->expectCall_eventHandler_onClose();
    this->expectCall_ioService_shutdownSocket();
    socket.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);
}

TEST_F(socketTest_t, receive) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                               eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_eventHandler_onReceive(eMUCore::network::maxPayloadSize_c);
    this->expectCall_ioService_read();
    ioService_.dequeueRead(boost::system::error_code(), eMUCore::network::maxPayloadSize_c);
}

TEST_F(socketTest_t, receive_Close) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
        eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_eventHandler_onClose();
    this->expectCall_ioService_shutdownSocket();
    ioService_.dequeueRead(boost::system::error_code(), 0);
}

TEST_F(socketTest_t, receive_Error) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
        eMUUnitTest::networkTest::socketStub_t> socket(ioService_, eventHandler_);

    this->expectCall_eventHandler_onClose();
    this->expectCall_ioService_shutdownSocket();
    ioService_.dequeueRead(boost::asio::error::bad_descriptor, 0);
}
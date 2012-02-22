#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../shared/types.hpp"
#include "socketStub.hpp"
#include "ioServiceMock.hpp"
#include "socketCallbackMock.hpp"
#include "../core/socket.hpp"

namespace eMUNetwork = eMUCore::network;
namespace eMUNetworkUT = eMUUnitTest::networkTest;

class socketTest_t: public ::testing::Test {
public:
    void SetUp() {
        this->initializePatternBuf();
        this->createSocket();
    }

    void TearDown() {}

    void initializePatternBuf() {
        uint8 value = 0;
        for(size_t i = 0; i < eMUNetwork::maxPayloadSize_c; ++i)
            patternBuf_.payload_[i] = value++;
    }

    void createSocket() {
        socket_.reset(new eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                          eMUNetworkUT::socketStub_t>(ioService_,
                                                      boost::bind(&eMUNetworkUT::socketCallbackMock_t::onReceive,
                                                                  &socketCallbackMock_,
                                                                  _1),
                                                       boost::bind(&eMUNetworkUT::socketCallbackMock_t::onClose,
                                                                   &socketCallbackMock_,
                                                                   _1)));
    }

    void expectCall_ioService_read() {
        EXPECT_CALL(ioService_, read(::testing::NotNull(), eMUNetwork::maxPayloadSize_c, ::testing::_));
    }

    void expectCall_ioService_write(size_t size) {
        EXPECT_CALL(ioService_, write(::testing::NotNull(), size, ::testing::_));
    }

    void expectCall_ioService_shutdownSocket() {
        EXPECT_CALL(ioService_, shutdownSocket(boost::asio::ip::tcp::socket::shutdown_both));
    }

    void expectCall_ioService_closeSocket() {
        EXPECT_CALL(ioService_, closeSocket());
    }

    void expectCall_socketCallback_onClose() {
        EXPECT_CALL(socketCallbackMock_, onClose(socket_));
    }

    void expectCall_socketCallback_onReceive() {
        EXPECT_CALL(socketCallbackMock_, onReceive(socket_));
    }

    eMUNetworkUT::ioServiceMock_t ioService_;
    eMUNetworkUT::socketCallbackMock_t socketCallbackMock_;
    eMUNetwork::readBuffer_t patternBuf_;
    eMUNetwork::socket_t<eMUNetworkUT::ioServiceMock_t,
                         eMUNetworkUT::socketStub_t>::ptr_t socket_;
};

TEST_F(socketTest_t, close) {
    this->expectCall_socketCallback_onClose();
    this->expectCall_ioService_shutdownSocket();
    this->expectCall_ioService_closeSocket();
    socket_->close();
}

TEST_F(socketTest_t, send) {
    ioService_.delegateMocks();

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);

    ioService_.dequeueWrite(boost::system::error_code(), eMUNetwork::maxPayloadSize_c);

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);
}

TEST_F(socketTest_t, send_Pending) {
    ioService_.delegateMocks();

    size_t testPayloadSize = eMUNetwork::maxPayloadSize_c / 2;

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);

    socket_->send(&patternBuf_.payload_[0], testPayloadSize);
    socket_->send(&patternBuf_.payload_[testPayloadSize], testPayloadSize);

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    ioService_.dequeueWrite(boost::system::error_code(), eMUNetwork::maxPayloadSize_c);
}

TEST_F(socketTest_t, send_Error) {
    ioService_.delegateMocks();

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);

    this->expectCall_socketCallback_onClose();
    this->expectCall_ioService_shutdownSocket();
    this->expectCall_ioService_closeSocket();
    ioService_.dequeueWrite(boost::asio::error::connection_reset, 0);
}

TEST_F(socketTest_t, send_Overflow) {
    ioService_.delegateMocks();

    this->expectCall_ioService_write(eMUNetwork::maxPayloadSize_c);
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);

    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);

    this->expectCall_socketCallback_onClose();
    this->expectCall_ioService_shutdownSocket();
    this->expectCall_ioService_closeSocket();
    socket_->send(&patternBuf_.payload_[0], eMUNetwork::maxPayloadSize_c);
}

TEST_F(socketTest_t, receive) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    socket_->queueRead();

    this->expectCall_socketCallback_onReceive();
    this->expectCall_ioService_read();
    ioService_.dequeueRead(boost::system::error_code(), eMUNetwork::maxPayloadSize_c);

    EXPECT_EQ(eMUNetwork::maxPayloadSize_c, socket_->rbuf().payloadSize_);

    this->expectCall_socketCallback_onReceive();
    this->expectCall_ioService_read();
    ioService_.dequeueRead(boost::system::error_code(), eMUNetwork::maxPayloadSize_c / 2);

    EXPECT_EQ(eMUNetwork::maxPayloadSize_c / 2, socket_->rbuf().payloadSize_);
}

TEST_F(socketTest_t, receive_Close) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    socket_->queueRead();

    this->expectCall_socketCallback_onClose();
    this->expectCall_ioService_shutdownSocket();
    this->expectCall_ioService_closeSocket();
    ioService_.dequeueRead(boost::system::error_code(), 0);

    EXPECT_EQ(0, socket_->rbuf().payloadSize_);
}

TEST_F(socketTest_t, receive_Error) {
    ioService_.delegateMocks();

    this->expectCall_ioService_read();
    socket_->queueRead();

    this->expectCall_socketCallback_onClose();
    this->expectCall_ioService_shutdownSocket();
    this->expectCall_ioService_closeSocket();
    ioService_.dequeueRead(boost::asio::error::bad_descriptor, 0);
}
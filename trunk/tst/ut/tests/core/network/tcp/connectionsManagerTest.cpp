#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connectionsManager.hpp>
#include <core/common/exception.hpp>
#include <ut/env/asioStub/ioService.hpp>
#include <ut/env/asioStub/tcp/acceptor.hpp>
#include <ut/env/asioStub/tcp/socket.hpp>
#include <ut/env/core/network/tcp/connectionsManagerEventsMock.hpp>

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::Throw;
using ::testing::DoAll;

ACTION_TEMPLATE(SaveArgToPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer)) {
  *pointer = &(::std::tr1::get<k>(args));
}

class ConnectionsManagerTest: public ::testing::Test {
public:
    ConnectionsManagerTest():
      connectionsManager_(ioService_, 55962) {
        connectionsManager_.setGenerateConnectionHashCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionsManagerEventsMock::generateConnectionHash,
                                                                        &connectionsManagerEventsMock_));

        connectionsManager_.setAcceptEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionsManagerEventsMock::acceptEvent,
                                                             &connectionsManagerEventsMock_,
                                                             std::placeholders::_1));

        connectionsManager_.setReceiveEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionsManagerEventsMock::receiveEvent,
                                                    &connectionsManagerEventsMock_,
                                                    std::placeholders::_1,
                                                    std::placeholders::_2));

        connectionsManager_.setCloseEventCallback(std::bind(&eMU::ut::env::core::tcp::ConnectionsManagerEventsMock::closeEvent,
                                                  &connectionsManagerEventsMock_,
                                                  std::placeholders::_1));
    }

    void expectAsyncAcceptCallAndSaveArguments() {
        EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _)).WillOnce(DoAll(SaveArgToPointer<0>(&socket_), SaveArg<1>(&acceptHandler_)));
    }

    void expectAsyncReceiveCallAndSaveCallback() {
        EXPECT_CALL(*socket_, async_receive(_, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_), SaveArg<1>(&receiveHandler_)));
    }

    eMU::ut::env::asioStub::ip::tcp::acceptor::AcceptHandler acceptHandler_;
    eMU::ut::env::asioStub::ip::tcp::socket *socket_;
    eMU::ut::env::asioStub::io_service ioService_;
    eMU::ut::env::core::tcp::ConnectionsManagerEventsMock connectionsManagerEventsMock_;
    eMU::core::network::tcp::ConnectionsManager connectionsManager_;
    eMU::ut::env::asioStub::io_service::IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;
};

TEST_F(ConnectionsManagerTest, triggerReceiveEvent) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    size_t connectionHash = 11111;
    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(connectionHash));
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(connectionHash));
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));
    expectAsyncReceiveCallAndSaveCallback();

    acceptHandler_(boost::system::error_code());

    eMU::core::network::Payload payload(100, 0x14);
    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &payload[0], payload.size());

    EXPECT_CALL(connectionsManagerEventsMock_, receiveEvent(connectionHash, payload));
    EXPECT_CALL(*socket_, async_receive(_, _));
    receiveHandler_(boost::system::error_code(), 100);
}

TEST_F(ConnectionsManagerTest, triggerCloseEvent) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    size_t connectionHash = 11111;
    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(connectionHash));
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(connectionHash));
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));
    expectAsyncReceiveCallAndSaveCallback();

    acceptHandler_(boost::system::error_code());

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionsManagerEventsMock_, closeEvent(connectionHash));
    receiveHandler_(boost::asio::error::eof, 0);
}

TEST_F(ConnectionsManagerTest, errorOccuredDuringConnectionAccept) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).Times(0);
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(_)).Times(0);
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));

    acceptHandler_(boost::asio::error::host_not_found_try_again);
}

TEST_F(ConnectionsManagerTest, exceptionOccuredDuringConnectionAccept) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    eMU::core::common::Exception exception; exception.in() << "Test";
    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Throw(exception));
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(_)).Times(0);
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));

    acceptHandler_(boost::system::error_code());
}

TEST_F(ConnectionsManagerTest, disconnectShouldTriggerCloseEvent) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    size_t connectionHash = 11111;
    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(connectionHash));
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(connectionHash));
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));
    expectAsyncReceiveCallAndSaveCallback();

    acceptHandler_(boost::system::error_code());

    EXPECT_CALL(*socket_, is_open()).WillOnce(Return(true));
    EXPECT_CALL(connectionsManagerEventsMock_, closeEvent(connectionHash));
    connectionsManager_.disconnect(connectionHash);
}

TEST_F(ConnectionsManagerTest, send) {
    expectAsyncAcceptCallAndSaveArguments();

    connectionsManager_.queueAccept();

    size_t connectionHash = 11111;
    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(connectionHash));
    EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(connectionHash));
    EXPECT_CALL(connectionsManager_.acceptor(), async_accept(_, _));
    expectAsyncReceiveCallAndSaveCallback();

    acceptHandler_(boost::system::error_code());

    boost::asio::mutable_buffer sendBuffer;
    EXPECT_CALL(*socket_, async_send(_, _)).WillOnce(SaveArg<0>(&sendBuffer));

    eMU::core::network::Payload payload(100, 0x14);
    connectionsManager_.send(connectionHash, payload);

    ASSERT_EQ(payload.size(), boost::asio::buffer_size(sendBuffer));
    int32_t result = memcmp(boost::asio::buffer_cast<const uint8_t*>(sendBuffer), &payload[0], payload.size());
    ASSERT_EQ(0, result);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/tcp/connectionsManager.hpp>
#include <ut/env/asioStub/ioService.hpp>
#include <ut/env/asioStub/tcp/acceptor.hpp>
#include <ut/env/core/network/tcp/connectionsManagerEventsMock.hpp>
#include <ut/env/core/network/tcp/connectionsFactoryMock.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;
using ::testing::Throw;
using ::testing::NotNull;
using ::testing::ReturnRef;
using ::testing::Ref;
using ::testing::SaveArg;

ACTION_TEMPLATE(SaveArgToPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer))
{
    *pointer = &(::std::tr1::get<k>(args));
}

namespace asioStub = eMU::ut::env::asioStub;
namespace network = eMU::core::network;
namespace tcpEnv = eMU::ut::env::core::tcp;

class ConnectionsManagerTest: public ::testing::Test
{
public:
    ConnectionsManagerTest():
        port_(55962),
        acceptor_(new asioStub::ip::tcp::acceptor(ioService_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_))),
        connectionsFactory_(new tcpEnv::ConnectionsFactoryMock()),
        connectionsManager_(connectionsFactory_, ioService_, acceptor_),
        connection_(tcpEnv::ConnectionMock::SocketPointer(new asioStub::ip::tcp::socket(ioService_))),
        connectionHash_(1234)
    {
        connectionsManager_.setGenerateConnectionHashCallback(std::bind(&tcpEnv::ConnectionsManagerEventsMock::generateConnectionHash,
                &connectionsManagerEventsMock_));

        connectionsManager_.setAcceptEventCallback(std::bind(&tcpEnv::ConnectionsManagerEventsMock::acceptEvent,
                &connectionsManagerEventsMock_,
                std::placeholders::_1));

        connectionsManager_.setReceiveEventCallback(std::bind(&tcpEnv::ConnectionsManagerEventsMock::receiveEvent,
                &connectionsManagerEventsMock_,
                std::placeholders::_1,
                std::placeholders::_2));

        connectionsManager_.setCloseEventCallback(std::bind(&tcpEnv::ConnectionsManagerEventsMock::closeEvent,
                &connectionsManagerEventsMock_,
                std::placeholders::_1));

        samplePayload_.setValue<uint32_t>(0, 0xFFFFFFFF);
        samplePayload_.setValue<uint32_t>(4, 0xEEEEEEEE);
    }

    void expectAsyncAcceptCallAndSaveArguments()
    {
        EXPECT_CALL(*acceptor_, async_accept(_, _)).WillOnce(SaveArg<1>(&acceptHandler_));
    }

    void acceptScenario()
    {
        expectAsyncAcceptCallAndSaveArguments();

        connectionsManager_.queueAccept();

        EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(connectionHash_));
        EXPECT_CALL(connectionsManagerEventsMock_, acceptEvent(connectionHash_));

        EXPECT_CALL(connection_, queueReceive());
        EXPECT_CALL(connection_, setReceiveEventCallback(_)).WillOnce(SaveArg<0>(&receiveCallback_));
        EXPECT_CALL(connection_, setCloseEventCallback(_)).WillOnce(SaveArg<0>(&closeCallback_));
        EXPECT_CALL(*connectionsFactory_, create(connectionHash_, NotNull())).WillOnce(ReturnRef(connection_));
        EXPECT_CALL(*acceptor_, async_accept(_, _));

        acceptHandler_(boost::system::error_code());
    }

    asioStub::io_service ioService_;
    uint16_t port_;
    network::tcp::ConnectionsManager::AcceptorPointer acceptor_;
    std::shared_ptr<tcpEnv::ConnectionsFactoryMock> connectionsFactory_;

    asioStub::ip::tcp::acceptor::AcceptHandler acceptHandler_;
    tcpEnv::ConnectionsManagerEventsMock connectionsManagerEventsMock_;
    network::tcp::ConnectionsManager connectionsManager_;
    tcpEnv::ConnectionMock connection_;
    size_t connectionHash_;

    network::tcp::Connection::EventCallback receiveCallback_;
    network::tcp::Connection::EventCallback closeCallback_;

    network::Payload samplePayload_;
};

TEST_F(ConnectionsManagerTest, accept)
{
    acceptScenario();
}

TEST_F(ConnectionsManagerTest, send)
{
    acceptScenario();

    eMU::core::network::Payload sentPayload;
    EXPECT_CALL(connection_, send(_)).WillOnce(SaveArg<0>(&sentPayload));
    EXPECT_CALL(*connectionsFactory_, get(connectionHash_)).WillOnce(ReturnRef(connection_));

    connectionsManager_.send(connectionHash_, samplePayload_);

    ASSERT_EQ(samplePayload_.getSize(), sentPayload.getSize());
    ASSERT_EQ(memcmp(&samplePayload_[0], &sentPayload[0], samplePayload_.getSize()), 0);
}

TEST_F(ConnectionsManagerTest, getThrowExceptionDuringSend)
{
    acceptScenario();

    EXPECT_CALL(*connectionsFactory_, get(connectionHash_)).WillOnce(Throw(network::tcp::ConnectionsFactory::UnknownConnectionException()));

    connectionsManager_.send(connectionHash_, samplePayload_);
}

TEST_F(ConnectionsManagerTest, disconnect)
{
    acceptScenario();

    EXPECT_CALL(connection_, disconnect());
    EXPECT_CALL(*connectionsFactory_, get(connectionHash_)).WillOnce(ReturnRef(connection_));

    connectionsManager_.disconnect(connectionHash_);

    EXPECT_CALL(*connectionsFactory_, getHash(Ref(connection_))).WillOnce(Return(connectionHash_));
    EXPECT_CALL(*connectionsFactory_, destroy(connectionHash_));
    EXPECT_CALL(connectionsManagerEventsMock_, closeEvent(connectionHash_));
    EXPECT_CALL(connection_, close());

    closeCallback_(connection_);
}

TEST_F(ConnectionsManagerTest, getThrowExceptionDuringDisconnect)
{
    acceptScenario();

    EXPECT_CALL(*connectionsFactory_, get(connectionHash_)).WillOnce(Throw(network::tcp::ConnectionsFactory::UnknownConnectionException()));

    connectionsManager_.disconnect(connectionHash_);
}

TEST_F(ConnectionsManagerTest, getHashThrowExceptionDuringCloseEvent)
{
    acceptScenario();

    EXPECT_CALL(connection_, disconnect());
    EXPECT_CALL(*connectionsFactory_, get(connectionHash_)).WillOnce(ReturnRef(connection_));
    connectionsManager_.disconnect(connectionHash_);

    EXPECT_CALL(*connectionsFactory_, getHash(Ref(connection_))).WillOnce(Throw(network::tcp::ConnectionsFactory::UnknownConnectionException()));
    closeCallback_(connection_);
}

TEST_F(ConnectionsManagerTest, errorOccuredDuringAccept)
{
    expectAsyncAcceptCallAndSaveArguments();
    connectionsManager_.queueAccept();

    EXPECT_CALL(*acceptor_, async_accept(_, _));
    acceptHandler_(boost::asio::error::host_not_found_try_again);
}

TEST_F(ConnectionsManagerTest, exceptionOccuredDuringGeneratingHash)
{
    expectAsyncAcceptCallAndSaveArguments();
    connectionsManager_.queueAccept();

    EXPECT_CALL(connectionsManagerEventsMock_, generateConnectionHash()).WillOnce(Return(0));
    EXPECT_CALL(*acceptor_, async_accept(_, _));

    acceptHandler_(boost::system::error_code());
}

TEST_F(ConnectionsManagerTest, receive)
{
    acceptScenario();

    EXPECT_CALL(*connectionsFactory_, getHash(Ref(connection_))).WillOnce(Return(connectionHash_));
    EXPECT_CALL(connectionsManagerEventsMock_, receiveEvent(connectionHash_, _));

    receiveCallback_(connection_);
}

TEST_F(ConnectionsManagerTest, getHashThrowExceptionDuringReceive)
{
    acceptScenario();

    EXPECT_CALL(*connectionsFactory_, getHash(Ref(connection_))).WillOnce(Throw(network::tcp::ConnectionsFactory::UnknownConnectionException()));

    receiveCallback_(connection_);
}

TEST_F(ConnectionsManagerTest, CheckConstructor)
{
    network::tcp::ConnectionsManager ConnectionsManager(ioService_, port_);
}

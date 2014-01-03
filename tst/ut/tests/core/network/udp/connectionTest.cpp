#include <core/network/udp/connection.hpp>
#include <core/network/udp/protocol.hpp>
#include <ut/env/asioStub/udp/socket.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::_;
using ::testing::NotNull;
using ::testing::ActionInterface;

namespace asioStub = eMU::ut::env::asioStub;
using asioStub::io_service;

using eMU::ut::env::core::network::SamplePayloads;

using eMU::core::network::udp::Protocol;
using eMU::core::network::udp::Connection;
using eMU::core::network::Payload;

class UdpConnectionTest: public ::testing::Test
{
public:
    class ProtocolMock: public Protocol
    {
    public:
        MOCK_METHOD1(attach, void(Connection::Pointer));
        MOCK_METHOD2(dispatch, void(Connection::Pointer, const boost::asio::ip::udp::endpoint&));
        MOCK_METHOD1(detach, void(Connection::Pointer));
    };

    UdpConnectionTest():
        connection_(new Connection(ioService_, 44405, protocol_)) {}

    ProtocolMock protocol_;
    asioStub::io_service ioService_;
    Connection::Pointer connection_;

    boost::asio::mutable_buffer receiveBuffer_;
    boost::asio::mutable_buffer sendBuffer_;

    SamplePayloads samplePayloads_;
};

TEST_F(UdpConnectionTest, receiveFrom)
{
    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(connection_->getSocket(), async_receive_from(_, _, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_),
                                                                                     SetArgReferee<1>(senderEndpoint)));

    connection_->queueReceiveFrom();

    ASSERT_THAT(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_), NotNull());
    ASSERT_EQ(boost::asio::buffer_size(receiveBuffer_), Payload::getMaxSize());

    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &samplePayloads_.payload1_[0], samplePayloads_.payload1_.getSize());

    EXPECT_CALL(connection_->getSocket(), async_receive_from(_, _, _));
    EXPECT_CALL(protocol_, dispatch(connection_, senderEndpoint));

    connection_->receiveFromHandler(boost::system::error_code(), samplePayloads_.payload1_.getSize());

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), connection_->getReadPayload().getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], &connection_->getReadPayload()[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(UdpConnectionTest, receiveFrom_error)
{
    EXPECT_CALL(connection_->getSocket(), async_receive_from(_, _, _));
    connection_->queueReceiveFrom();

    EXPECT_CALL(connection_->getSocket(), async_receive_from(_, _, _));

    connection_->receiveFromHandler(boost::asio::error::fault, 0);
}

TEST_F(UdpConnectionTest, sendTo)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&sendBuffer_));

    connection_->sendTo(endpoint, samplePayloads_.payload1_);

    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), samplePayloads_.payload1_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(UdpConnectionTest, sendTo_pending)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    size_t attempts = 3;

    for(size_t i = 0; i < attempts; ++i)
    {
        boost::asio::mutable_buffer sendBuffer1;
        asioStub::io_service::IoHandler sendToHandler1;

        EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&sendBuffer1));
        connection_->sendTo(endpoint, samplePayloads_.payload1_);

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer1), samplePayloads_.payload1_.getSize());
        ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.payload1_.getSize()), 0);

        connection_->sendTo(endpoint, samplePayloads_.halfFilledPayload_);

        connection_->sendTo(endpoint, samplePayloads_.halfFilledPayload_);

        boost::asio::mutable_buffer sendBuffer2;        
        EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&sendBuffer2));

        connection_->sendToHandler(endpoint, boost::system::error_code(), samplePayloads_.payload1_.getSize());

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer2), samplePayloads_.fullFilledPayload_.getSize());
        ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.fullFilledPayload_.getSize()), 0);

        connection_->sendToHandler(endpoint, boost::system::error_code(), samplePayloads_.fullFilledPayload_.getSize());
    }
}

TEST_F(UdpConnectionTest, sendTo_error)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _));

    connection_->sendTo(endpoint, samplePayloads_.payload2_);

    connection_->sendToHandler(endpoint, boost::asio::error::connection_reset, 0);
}

TEST_F(UdpConnectionTest, sendTo_error_with_pending)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _));

    connection_->sendTo(endpoint, samplePayloads_.payload1_);
    connection_->sendTo(endpoint, samplePayloads_.payload2_);

    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&sendBuffer_));

    connection_->sendToHandler(endpoint, boost::asio::error::connection_reset, 0);

    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), samplePayloads_.payload2_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload2_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), samplePayloads_.payload2_.getSize()), 0);

    connection_->sendToHandler(endpoint, boost::system::error_code(), samplePayloads_.payload2_.getSize());
}

TEST_F(UdpConnectionTest, sendTo_different_endpoints)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _));

    connection_->sendTo(endpoint, samplePayloads_.payload1_);

    connection_->sendTo(endpoint, samplePayloads_.payload2_);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint2, _));

    connection_->sendTo(endpoint2, samplePayloads_.payload3_);
}

TEST_F(UdpConnectionTest, sendTo_overflow_buffer)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _));
    connection_->sendTo(endpoint, samplePayloads_.payload2_);

    connection_->sendTo(endpoint, samplePayloads_.halfFilledPayload_);
    connection_->sendTo(endpoint, samplePayloads_.fullFilledPayload_);

    boost::asio::mutable_buffer pendingBuffer;
    EXPECT_CALL(connection_->getSocket(), async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&pendingBuffer));

    connection_->sendToHandler(endpoint, boost::system::error_code(), samplePayloads_.payload2_.getSize());

    ASSERT_EQ(boost::asio::buffer_size(pendingBuffer), samplePayloads_.halfFilledPayload_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.halfFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(pendingBuffer), samplePayloads_.halfFilledPayload_.getSize()), 0);
}

TEST_F(UdpConnectionTest, CheckConstructor)
{
    Connection connection(ioService_, 44405, protocol_);
}

TEST_F(UdpConnectionTest, ConnectionShouldBeRegisteredInProtocol)
{
    EXPECT_CALL(protocol_, attach(connection_));

    connection_->registerConnection();
}

TEST_F(UdpConnectionTest, ConnectionShouldBeUnregisteredInProtocol)
{
    EXPECT_CALL(protocol_, detach(connection_));

    connection_->unregisterConnection();
}

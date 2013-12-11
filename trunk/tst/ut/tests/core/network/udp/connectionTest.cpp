#include <core/network/udp/connection.hpp>
#include <ut/env/asioStub/udp/socket.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::_;
using ::testing::NotNull;
using ::testing::ActionInterface;

namespace asioStub = eMU::ut::env::asioStub;
using asioStub::io_service;

using eMU::ut::env::core::network::SamplePayloads;

using eMU::core::network::udp::Connection;
using eMU::core::network::Payload;

ACTION_TEMPLATE(SaveArgToPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(pointer))
{
    *pointer = &(::std::tr1::get<k>(args));
}

class UdpConnectionTest: public ::testing::Test
{
public:
    UdpConnectionTest():
        port_(44405),
        socket_(new asioStub::ip::udp::socket(ioService_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port_))),
        connection_(socket_) {}

    void SetUp()
    {
        connection_.setReceiveFromEventCallback(std::bind(&UdpConnectionTest::receiveFromEvent,
                                                          this,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2));
    }

    void TearDown() {}

    void expectAsyncReceiveFromCallAndSaveArguments()
    {
        EXPECT_CALL(*socket_, async_receive_from(_, _, _)).WillOnce(DoAll(SaveArg<0>(&receiveBuffer_),
                SaveArgToPointer<1>(&senderEndpoint_),
                SaveArg<2>(&receiveFromHandler_)));
    }

    void expectAsyncSendToCallAndSaveArguments(const boost::asio::ip::udp::endpoint &expectedEndpoint)
    {
        EXPECT_CALL(*socket_, async_send_to(_, expectedEndpoint, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer_),
                SaveArg<2>(&sendToHandler_)));
    }

    MOCK_METHOD2(receiveFromEvent, void(Connection&, const boost::asio::ip::udp::endpoint&));

    uint16_t port_;
    Connection::SocketPointer socket_;
    Connection connection_;
    asioStub::io_service ioService_;

    boost::asio::mutable_buffer receiveBuffer_;
    io_service::IoHandler receiveFromHandler_;
    boost::asio::ip::udp::endpoint* senderEndpoint_;

    boost::asio::mutable_buffer sendBuffer_;
    asioStub::io_service::IoHandler sendToHandler_;

    SamplePayloads samplePayloads_;
};

TEST_F(UdpConnectionTest, receiveFrom)
{
    expectAsyncReceiveFromCallAndSaveArguments();

    connection_.queueReceiveFrom();

    ASSERT_THAT(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_), NotNull());
    ASSERT_EQ(boost::asio::buffer_size(receiveBuffer_), Payload::getMaxSize());

    memcpy(boost::asio::buffer_cast<uint8_t*>(receiveBuffer_), &samplePayloads_.payload1_[0], samplePayloads_.payload1_.getSize());

    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    *senderEndpoint_ = senderEndpoint;

    EXPECT_CALL(*socket_, async_receive_from(_, _, _));
    EXPECT_CALL(*this, receiveFromEvent(::testing::Ref(connection_), senderEndpoint));

    receiveFromHandler_(boost::system::error_code(), samplePayloads_.payload1_.getSize());

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), connection_.getReadPayload().getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], &connection_.getReadPayload()[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(UdpConnectionTest, receiveFrom_error)
{
    expectAsyncReceiveFromCallAndSaveArguments();

    connection_.queueReceiveFrom();

    EXPECT_CALL(*socket_, async_receive_from(_, _, _));

    receiveFromHandler_(boost::asio::error::fault, 0);
}

TEST_F(UdpConnectionTest, sendTo)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    expectAsyncSendToCallAndSaveArguments(endpoint);

    connection_.sendTo(endpoint, samplePayloads_.payload1_);

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

        EXPECT_CALL(*socket_, async_send_to(_, endpoint, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer1),
                SaveArg<2>(&sendToHandler1)));

        connection_.sendTo(endpoint, samplePayloads_.payload1_);

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer1), samplePayloads_.payload1_.getSize());
        ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.payload1_.getSize()), 0);

        connection_.sendTo(endpoint, samplePayloads_.halfFilledPayload_);

        connection_.sendTo(endpoint, samplePayloads_.halfFilledPayload_);

        boost::asio::mutable_buffer sendBuffer2;
        asioStub::io_service::IoHandler sendToHandler2;

        EXPECT_CALL(*socket_, async_send_to(_, endpoint, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer2),
                SaveArg<2>(&sendToHandler2)));

        sendToHandler1(boost::system::error_code(), samplePayloads_.payload1_.getSize());

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer2), samplePayloads_.fullFilledPayload_.getSize());
        ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer1), samplePayloads_.fullFilledPayload_.getSize()), 0);

        sendToHandler2(boost::system::error_code(), samplePayloads_.fullFilledPayload_.getSize());
    }
}

TEST_F(UdpConnectionTest, sendTo_error)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    expectAsyncSendToCallAndSaveArguments(endpoint);

    connection_.sendTo(endpoint, samplePayloads_.payload2_);

    sendToHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(UdpConnectionTest, sendTo_error_with_pending)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    expectAsyncSendToCallAndSaveArguments(endpoint);

    connection_.sendTo(endpoint, samplePayloads_.payload1_);
    connection_.sendTo(endpoint, samplePayloads_.payload2_);

    expectAsyncSendToCallAndSaveArguments(endpoint);

    sendToHandler_(boost::asio::error::connection_reset, 0);

    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), samplePayloads_.payload2_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload2_[0], boost::asio::buffer_cast<const uint8_t*>(sendBuffer_), samplePayloads_.payload2_.getSize()), 0);

    sendToHandler_(boost::system::error_code(), samplePayloads_.payload2_.getSize());
}

TEST_F(UdpConnectionTest, sendTo_different_endpoints)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(*socket_, async_send_to(_, endpoint, _));

    connection_.sendTo(endpoint, samplePayloads_.payload1_);

    connection_.sendTo(endpoint, samplePayloads_.payload2_);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    EXPECT_CALL(*socket_, async_send_to(_, endpoint2, _));

    connection_.sendTo(endpoint2, samplePayloads_.payload3_);
}

TEST_F(UdpConnectionTest, sendTo_overflow_buffer)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    expectAsyncSendToCallAndSaveArguments(endpoint);
    connection_.sendTo(endpoint, samplePayloads_.payload2_);

    connection_.sendTo(endpoint, samplePayloads_.halfFilledPayload_);
    connection_.sendTo(endpoint, samplePayloads_.fullFilledPayload_);

    boost::asio::mutable_buffer pendingBuffer;
    EXPECT_CALL(*socket_, async_send_to(_, endpoint, _)).WillOnce(SaveArg<0>(&pendingBuffer));

    sendToHandler_(boost::system::error_code(), samplePayloads_.payload2_.getSize());

    ASSERT_EQ(boost::asio::buffer_size(pendingBuffer), samplePayloads_.halfFilledPayload_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.halfFilledPayload_[0], boost::asio::buffer_cast<const uint8_t*>(pendingBuffer), samplePayloads_.halfFilledPayload_.getSize()), 0);
}

TEST_F(UdpConnectionTest, CheckConstructor)
{
    Connection connection(ioService_, port_);
}

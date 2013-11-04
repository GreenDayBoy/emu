#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/network/udp/connection.hpp>
#include <ut/env/core/network/udp/connectionEventsMock.hpp>
#include <ut/env/asioStub/udp/socket.hpp>

using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::_;
using ::testing::NotNull;
using ::testing::ActionInterface;

namespace asioStub = eMU::ut::env::asioStub;
namespace network = eMU::core::network;
namespace udpEnv = eMU::ut::env::core::udp;

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
        connection_.setReceiveFromEventCallback(std::bind(&eMU::ut::env::core::udp::ConnectionEventsMock::receiveFromEvent,
                                                &connectionEvents_,
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

    void insertFakePayload(const network::Payload &payload, boost::asio::mutable_buffer &buffer)
    {
        memcpy(boost::asio::buffer_cast<uint8_t*>(buffer), &payload[0], payload.size());
    }

    bool isPayloadTheSame(const network::Payload &payload, const uint8_t *sourceBuffer)
    {
        return memcmp(&payload[0], sourceBuffer, payload.size()) == 0;
    }

    uint16_t port_;
    network::udp::Connection::SocketPointer socket_;
    network::udp::Connection connection_;
    asioStub::io_service ioService_;
    udpEnv::ConnectionEventsMock connectionEvents_;

    boost::asio::mutable_buffer receiveBuffer_;
    asioStub::io_service::IoHandler receiveFromHandler_;
    boost::asio::ip::udp::endpoint* senderEndpoint_;

    boost::asio::mutable_buffer sendBuffer_;
    asioStub::io_service::IoHandler sendToHandler_;
};

TEST_F(UdpConnectionTest, receiveFrom)
{
    expectAsyncReceiveFromCallAndSaveArguments();

    connection_.queueReceiveFrom();

    ASSERT_THAT(boost::asio::buffer_cast<const uint8_t*>(receiveBuffer_), NotNull());
    ASSERT_EQ(boost::asio::buffer_size(receiveBuffer_), network::kMaxPayloadSize);

    network::Payload payload(100, 0x14);
    insertFakePayload(payload, receiveBuffer_);

    boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);

    *senderEndpoint_ = senderEndpoint;

    EXPECT_CALL(*socket_, async_receive_from(_, _, _));
    EXPECT_CALL(connectionEvents_, receiveFromEvent(::testing::Ref(connection_), senderEndpoint));

    receiveFromHandler_(boost::system::error_code(), payload.size());

    ASSERT_EQ(payload.size(), connection_.readBuffer().payloadSize_);
    EXPECT_TRUE(isPayloadTheSame(payload, &connection_.readBuffer().payload_[0]));
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
    network::Payload payload(100, 0x14);

    expectAsyncSendToCallAndSaveArguments(endpoint);

    connection_.sendTo(endpoint, payload);

    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), payload.size());
    EXPECT_TRUE(isPayloadTheSame(payload, boost::asio::buffer_cast<const uint8_t*>(sendBuffer_)));
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
        network::Payload payload1(100, 0x10);
        connection_.sendTo(endpoint, payload1);

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer1), payload1.size());
        EXPECT_TRUE(isPayloadTheSame(payload1, boost::asio::buffer_cast<const uint8_t*>(sendBuffer1)));

        network::Payload payload2(100, 0x11);
        connection_.sendTo(endpoint, payload2);

        network::Payload payload3(100, 0x12);
        connection_.sendTo(endpoint, payload3);

        boost::asio::mutable_buffer sendBuffer2;
        asioStub::io_service::IoHandler sendToHandler2;

        EXPECT_CALL(*socket_, async_send_to(_, endpoint, _)).WillOnce(DoAll(SaveArg<0>(&sendBuffer2),
                SaveArg<2>(&sendToHandler2)));

        sendToHandler1(boost::system::error_code(), payload1.size());

        payload2.insert(payload2.end(), payload3.begin(), payload3.end());

        ASSERT_EQ(boost::asio::buffer_size(sendBuffer2), payload2.size());
        EXPECT_TRUE(isPayloadTheSame(payload2, boost::asio::buffer_cast<const uint8_t*>(sendBuffer2)));

        sendToHandler2(boost::system::error_code(), payload2.size());
    }
}

TEST_F(UdpConnectionTest, sendTo_error)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    network::Payload payload(100, 0x10);

    expectAsyncSendToCallAndSaveArguments(endpoint);

    connection_.sendTo(endpoint, payload);

    sendToHandler_(boost::asio::error::connection_reset, 0);
}

TEST_F(UdpConnectionTest, sendTo_error_with_pending)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    expectAsyncSendToCallAndSaveArguments(endpoint);

    network::Payload payload(100, 0x10);
    connection_.sendTo(endpoint, payload);

    network::Payload payload2(100, 0x11);
    connection_.sendTo(endpoint, payload2);

    expectAsyncSendToCallAndSaveArguments(endpoint);

    sendToHandler_(boost::asio::error::connection_reset, 0);

    ASSERT_EQ(boost::asio::buffer_size(sendBuffer_), payload2.size());
    EXPECT_TRUE(isPayloadTheSame(payload2, boost::asio::buffer_cast<const uint8_t*>(sendBuffer_)));

    sendToHandler_(boost::system::error_code(), payload2.size());
}

TEST_F(UdpConnectionTest, sendTo_different_endpoints)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(*socket_, async_send_to(_, endpoint, _));

    network::Payload payload(100, 0x10);
    connection_.sendTo(endpoint, payload);

    network::Payload payload2(100, 0x10);
    connection_.sendTo(endpoint, payload2);

    boost::asio::ip::udp::endpoint endpoint2(boost::asio::ip::address::from_string("4.3.2.1"), 1234);
    EXPECT_CALL(*socket_, async_send_to(_, endpoint2, _));

    network::Payload payload3(100, 0x12);
    connection_.sendTo(endpoint2, payload3);
}

TEST_F(UdpConnectionTest, sendTo_overflow_buffer)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    EXPECT_CALL(*socket_, async_send_to(_, endpoint, _)).Times(0);

    network::Payload payload(network::kMaxPayloadSize + 1, 0x10);
    connection_.sendTo(endpoint, payload);
}

TEST_F(UdpConnectionTest, CheckConstructor)
{
    network::udp::Connection connection(ioService_, port_);
}

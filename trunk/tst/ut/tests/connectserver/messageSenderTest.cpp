#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <connectserver/messageSender.hpp>
#include <interface/gameServerAddressResponse.hpp>
#include <interface/gameServersListResponse.hpp>
#include <interface/ids.hpp>
#include <core/protocol/helpers.hpp>

using ::testing::SaveArg;
using ::testing::DoAll;
using ::testing::_;

namespace interface = eMU::interface;

class MessageSenderTest: public ::testing::Test
{
protected:
    MOCK_METHOD2(send, void(size_t hash, const eMU::core::network::Payload&));

    MessageSenderTest():
        messageSender_(std::bind(&MessageSenderTest::send, this, std::placeholders::_1, std::placeholders::_2)),
        hash_(12345) {}

    eMU::connectserver::MessageSender messageSender_;
    size_t hash_;

};

TEST_F(MessageSenderTest, sendGameServerAddressResponse)
{
    size_t receivedHash = 0;
    eMU::core::network::Payload payload;
    std::string address = "127.0.0.2";
    uint16_t port = 50444;

    EXPECT_CALL(*this, send(_, _)).WillOnce(DoAll(SaveArg<0>(&receivedHash), SaveArg<1>(&payload)));
    messageSender_.sendGameServerAddressResponse(hash_, address, port);

    EXPECT_EQ(receivedHash, hash_);
    ASSERT_EQ(sizeof(interface::GameServerAddressResponse), payload.size());

    eMU::interface::GameServerAddressResponse *message = reinterpret_cast<interface::GameServerAddressResponse*>(&payload[0]);

    EXPECT_EQ(interface::MessageType::SMALL_DECRYPTED, message->header_.typeId_);
    EXPECT_EQ(sizeof(interface::GameServerAddressResponse), message->header_.size_);
    EXPECT_EQ(interface::ProtocolId::CONNECT_SERVER_PROTOCOL, message->header_.protocolId_);
    EXPECT_EQ(interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE, message->id_);

    EXPECT_EQ(0, memcmp(message->address_, address.c_str(), address.size()));
    EXPECT_EQ(port, message->port_);
}


TEST_F(MessageSenderTest, sendGameServersListResponse)
{
    size_t receivedHash = 0;
    eMU::core::network::Payload payload;
    eMU::connectserver::GameServersList::GameServersListContainer sampleServers = {{20, "Test1", "127.0.0.2", 55901, 100},
                                                                                   {0,  "Test2", "127.0.0.2", 55903, 30}};

    EXPECT_CALL(*this, send(_, _)).WillOnce(DoAll(SaveArg<0>(&receivedHash), SaveArg<1>(&payload)));
    messageSender_.sendGameServersListResponse(hash_, sampleServers);

    EXPECT_EQ(receivedHash, hash_);

    size_t messageSize = (sizeof(interface::GameServersListResponse) -
                          interface::kMaxNumberOfGameServers * sizeof(interface::GameServerInfo)) +
                          sampleServers.size() * sizeof(interface::GameServerInfo);
    ASSERT_EQ(messageSize, payload.size());

    interface::GameServersListResponse *message = reinterpret_cast<interface::GameServersListResponse*>(&payload[0]);

    EXPECT_EQ(eMU::interface::MessageType::LARGE_DECRYPTED, message->header_.typeId_);
    EXPECT_EQ(eMU::core::protocol::byteSwap(messageSize), message->header_.size_);
    EXPECT_EQ(eMU::interface::ProtocolId::CONNECT_SERVER_PROTOCOL, message->header_.protocolId_);
    EXPECT_EQ(eMU::interface::MessageId::GAME_SERVERS_LIST_RESPONSE, message->id_);

    ASSERT_EQ(sampleServers.size(), message->numberOfServers_);

    for(size_t i = 0; i < message->numberOfServers_; ++i)
    {
        EXPECT_EQ(sampleServers[i].code_, message->servers_[i].code_);
        EXPECT_EQ(sampleServers[i].load_, message->servers_[i].load_);
    }
}

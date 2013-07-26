#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <connectserver/messageSender.hpp>
#include <interface/gameServerAddressResponse.hpp>
#include <interface/protocolId.hpp>

using ::testing::SaveArg;
using ::testing::DoAll;
using ::testing::_;

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
    ASSERT_EQ(sizeof(eMU::interface::GameServerAddressResponse), payload.size());

    eMU::interface::GameServerAddressResponse *message = reinterpret_cast<eMU::interface::GameServerAddressResponse*>(&payload[0]);

    EXPECT_EQ(eMU::interface::MessageType::SMALL_DECRYPTED, message->header_.typeId_);
    EXPECT_EQ(sizeof(eMU::interface::GameServerAddressResponse), message->header_.size_);
    EXPECT_EQ(eMU::interface::ProtocolId::GAME_SERVER_ADDRESS_RESPONSE, message->header_.protocolId_);
    EXPECT_EQ(0x03, message->subProtocolId_);

    EXPECT_EQ(0, memcmp(message->address_, address.c_str(), address.size()));
    EXPECT_EQ(port, message->port_);
}

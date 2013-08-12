#include <mt/env/messages/verifiers/gameServerAddressResponseVerifier.hpp>
#include <interface/gameServerAddressResponse.hpp>
#include <interface/messageIds.hpp>
#include <interface/protocolIds.hpp>
#include <interface/messageTypes.hpp>
#include <core/protocol/exceptions.hpp>

#include <gtest/gtest.h>

namespace eMU
{
namespace mt
{
namespace env
{
namespace messages
{
namespace verifiers
{

void GameServerAddressResponseVerifier::operator()(const core::network::Payload &payload, std::string address, uint16_t port)
{
    if(payload.empty())
    {
        throw core::protocol::exceptions::EmptyPayloadException();
    }

    const interface::GameServerAddressResponse *message = reinterpret_cast<const interface::GameServerAddressResponse*>(&payload[0]);

    ASSERT_EQ(interface::MessageType::SMALL_DECRYPTED, message->header_.typeId_);
    ASSERT_EQ(sizeof(interface::GameServerAddressResponse), message->header_.size_);
    ASSERT_EQ(interface::ProtocolId::CONNECT_SERVER_PROTOCOL, message->header_.protocolId_);

    ASSERT_EQ(interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE, message->id_);

    EXPECT_EQ(address, message->address_);
    EXPECT_EQ(port, message->port_);
}

}
}
}
}
}

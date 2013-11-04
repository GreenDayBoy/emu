#include <mt/env/messages/verifiers/handshakeIndicationVerifier.hpp>
#include <interface/handshakeIndication.hpp>
#include <interface/messageIds.hpp>
#include <interface/protocolIds.hpp>
#include <interface/messageTypes.hpp>
#include <core/protocol/helpers.hpp>

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

void HandshakeIndicationVerifier::operator()(const core::network::Payload &payload)
{
    const interface::HandshakeIndication *message = reinterpret_cast<const interface::HandshakeIndication*>(&payload[0]);

    ASSERT_EQ(interface::MessageType::SMALL_DECRYPTED, message->header_.typeId_);
    ASSERT_EQ(sizeof(interface::HandshakeIndication), message->header_.size_);
    ASSERT_EQ(0x00, message->header_.protocolId_);
    ASSERT_EQ(interface::MessageId::HANDSHAKE_INDICATION, message->id_);

}

}
}
}
}
}

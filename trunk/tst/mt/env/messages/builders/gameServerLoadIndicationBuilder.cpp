#include <mt/env/messages/builders/gameServerLoadIndicationBuilder.hpp>
#include <interface/gameServerLoadIndication.hpp>
#include <interface/messageIds.hpp>
#include <interface/protocolIds.hpp>
#include <interface/messageTypes.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace messages
{
namespace builders
{
eMU::core::network::Payload GameServerLoadIndicationBuilder::operator()(uint16_t serverCode, uint8_t load)
{
    core::network::Payload payload;
    payload.resize(sizeof(interface::GameServerLoadIndication));
    interface::GameServerLoadIndication *message = reinterpret_cast<interface::GameServerLoadIndication*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::SMALL_DECRYPTED;
    message->header_.size_ = sizeof(interface::GameServerLoadIndication);
    message->header_.protocolId_ = interface::ProtocolId::CONNECT_SERVER_PROTOCOL;
    message->id_ = interface::MessageId::GAME_SERVER_LOAD_INDICATION;
    message->serverCode_ = serverCode;
    message->load_ = load;

    return payload;
}

}
}
}
}
}

#include <mt/env/messages/builders/gameServerAddressRequestBuilder.hpp>
#include <interface/gameServerAddressRequest.hpp>
#include <interface/messageIds.hpp>
#include <interface/messageTypes.hpp>
#include <interface/protocolIds.hpp>

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

eMU::core::network::Payload GameServerAddressRequestBuilder::operator()(uint16_t serverCode)
{
    core::network::Payload payload;
    payload.resize(sizeof(interface::GameServerAddressRequest));
    interface::GameServerAddressRequest *message = reinterpret_cast<interface::GameServerAddressRequest*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::SMALL_DECRYPTED;
    message->header_.size_ = sizeof(interface::GameServerAddressRequest);
    message->header_.protocolId_ = interface::ProtocolId::CONNECT_SERVER_PROTOCOL;
    message->id_ = interface::MessageId::GAME_SERVER_ADDRESS_REQUEST;
    message->serverCode_ = serverCode;

    return payload;
}

}
}
}
}
}

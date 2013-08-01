#include <mt/env/messages/builders/gameServersListRequestBuilder.hpp>
#include <interface/gameServersListRequest.hpp>
#include <interface/messageTypes.hpp>
#include <interface/messageIds.hpp>
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
eMU::core::network::Payload GameServersListRequestBuilder::operator()()
{
    core::network::Payload payload;
    payload.resize(sizeof(interface::GameServersListRequest));
    interface::GameServersListRequest *message = reinterpret_cast<interface::GameServersListRequest*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::SMALL_DECRYPTED;
    message->header_.size_ = static_cast<uint8_t>(sizeof(interface::GameServersListRequest));
    message->header_.protocolId_ = interface::ProtocolId::CONNECT_SERVER_PROTOCOL;
    message->id_ = interface::MessageId::GAME_SERVERS_LIST_REQUEST;

    return payload;
}

}
}
}
}
}

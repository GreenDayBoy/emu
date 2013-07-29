#include <connectserver/messageSender.hpp>
#include <interface/gameServerAddressResponse.hpp>
#include <interface/ids.hpp>
#include <interface/gameServersListResponse.hpp>
#include <core/protocol/helpers.hpp>

namespace eMU
{
namespace connectserver
{

MessageSender::MessageSender(const SendFunctor &sendFunctor):
    sendFunctor_(sendFunctor) {}
MessageSender::~MessageSender() {}

void MessageSender::sendGameServersListResponse(size_t hash, const GameServersList::GameServersListContainer &servers)
{
    size_t serverDetailsListSize = servers.size() * sizeof(interface::GameServerInfo);
    size_t maxServerDetailsListSize = interface::kMaxNumberOfGameServers * sizeof(interface::GameServerInfo);
    size_t messageSize = (sizeof(interface::GameServersListResponse) - maxServerDetailsListSize) + serverDetailsListSize;

    core::network::Payload payload; payload.resize(messageSize);
    interface::GameServersListResponse *message = reinterpret_cast<interface::GameServersListResponse*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::LARGE_DECRYPTED;
    message->header_.size_ = core::protocol::byteSwap(static_cast<uint16_t>(messageSize));
    message->header_.protocolId_ = interface::ProtocolId::CONNECT_SERVER_PROTOCOL;
    message->id_ = interface::MessageId::GAME_SERVERS_LIST_RESPONSE;
    message->numberOfServers_ = static_cast<uint16_t>(servers.size());

    for(size_t i = 0; i < servers.size(); ++i)
    {
        message->servers_[i].code_ = servers[i].code_;
        message->servers_[i].load_ = servers[i].load_;
    }

    sendFunctor_(hash, payload);
}

void MessageSender::sendGameServerAddressResponse(size_t hash, const std::string &address, uint16_t port)
{
    core::network::Payload payload; payload.resize(sizeof(interface::GameServerAddressResponse));

    interface::GameServerAddressResponse *message = reinterpret_cast<interface::GameServerAddressResponse*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::SMALL_DECRYPTED;
    message->header_.size_ = static_cast<uint8_t>(payload.size());
    message->header_.protocolId_ = interface::ProtocolId::CONNECT_SERVER_PROTOCOL;
    message->id_ = interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE;
    memcpy(message->address_, address.c_str(), address.size());
    message->port_ = port;

    sendFunctor_(hash, payload);
}

}
}

#include <connectserver/messageSender.hpp>
#include <interface/gameServerAddressResponse.hpp>
#include <interface/protocolId.hpp>

namespace eMU
{
namespace connectserver
{

MessageSender::MessageSender(const SendFunctor &sendFunctor):
    sendFunctor_(sendFunctor) {}
MessageSender::MessageSender() {}
MessageSender::~MessageSender() {}

void MessageSender::sendGameServersListResponse(size_t hash, const GameServersList::GameServersListContainer &servers)
{

}

void MessageSender::sendGameServerAddressResponse(size_t hash, const std::string &address, uint16_t port)
{
    core::network::Payload payload; payload.resize(sizeof(interface::GameServerAddressResponse));

    interface::GameServerAddressResponse *message = reinterpret_cast<interface::GameServerAddressResponse*>(&payload[0]);

    message->header_.typeId_ = interface::MessageType::SMALL_DECRYPTED;
    message->header_.size_ = static_cast<uint8_t>(payload.size());
    message->header_.protocolId_ = interface::ProtocolId::GAME_SERVER_ADDRESS_RESPONSE;
    message->subProtocolId_ = 0x03;
    memcpy(message->address_, address.c_str(), address.size());
    message->port_ = port;

    sendFunctor_(hash, payload);
}

}
}

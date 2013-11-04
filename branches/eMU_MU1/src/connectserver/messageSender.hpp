#pragma once

#include <connectserver/gameServersList.hpp>
#include <core/network/buffer.hpp>
#include <common/mockable.hpp>

namespace eMU
{
namespace connectserver
{

class MessageSender
{
public:
    typedef std::function<void(size_t, const core::network::Payload&)> SendFunctor;

    MessageSender(const SendFunctor &sendFunctor);
    virtual ~MessageSender();

    MOCKABLE void sendGameServersListResponse(size_t hash, const GameServersList::GameServersListContainer &servers);
    MOCKABLE void sendGameServerAddressResponse(size_t hash, const std::string &address, uint16_t port);
    MOCKABLE void sendHandshakeIndication(size_t hash);

private:
    MessageSender();

    SendFunctor sendFunctor_;
};

}
}

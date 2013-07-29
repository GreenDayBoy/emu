#include <glog/logging.h>
#include <connectserver/transactions/gameServerAddressResponseTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServerAddressResponseTransaction::GameServerAddressResponseTransaction(size_t hash,
                                                                           MessageSender &messageSender,
                                                                           const GameServersList &gameServersList,
                                                                           uint16_t code):
    hash_(hash),
    messageSender_(messageSender),
    gameServersList_(gameServersList),
    code_(code) {}

bool GameServerAddressResponseTransaction::validate() const
{
    if(!gameServersList_.hasGameServer(code_))
    {
        LOG(ERROR) << "hash: " << hash_ << ", Validate transaction failed, serverCode: " << code_;
        return false;
    }

    return true;
}

void GameServerAddressResponseTransaction::handleSelf()
{
    const GameServersList::GameServerInfo &serverInfo = gameServersList_.getGameServerInfo(code_);

    LOG(INFO) << "hash: " << hash_ << ", chosen gameserver name: " << serverInfo.name_;

    messageSender_.sendGameServerAddressResponse(hash_, serverInfo.address_, serverInfo.port_);
}

}
}
}

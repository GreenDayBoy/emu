#include <glog/logging.h>
#include <connectserver/transactions/gameServerAddressRequestTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServerAddressRequestTransaction::GameServerAddressRequestTransaction(size_t hash,
                                                                         MessageSender &messageSender,
                                                                         const GameServersList &gameServersList,
                                                                         uint16_t code):
    hash_(hash),
    messageSender_(messageSender),
    gameServersList_(gameServersList),
    code_(code) {}

bool GameServerAddressRequestTransaction::validate() const
{
    if(!gameServersList_.hasGameServer(code_))
    {
        LOG(ERROR) << "hash: " << hash_ << ", Validate transaction failed, serverCode: " << code_;
        return false;
    }

    return true;
}

void GameServerAddressRequestTransaction::handleSelf()
{
    const GameServersList::GameServerInfo &serverInfo = gameServersList_.getGameServerInfo(code_);

    LOG(INFO) << "hash: " << hash_ << ", chosen gameserver name: " << serverInfo.name_;

    messageSender_.sendGameServerAddressResponse(hash_, serverInfo.address_, serverInfo.port_);
}

}
}
}

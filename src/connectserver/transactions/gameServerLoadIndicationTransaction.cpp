#include <glog/logging.h>
#include <connectserver/transactions/gameServerLoadIndicationTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServerLoadIndicationTransaction::GameServerLoadIndicationTransaction(const interface::GameServerLoadIndication &message, GameServersList &gameServersList):
    message_(message),
    gameServersList_(gameServersList) {}

bool GameServerLoadIndicationTransaction::validate() const
{
    if(!gameServersList_.hasGameServer(message_.serverCode_))
    {
        LOG(ERROR) << "code: " << message_.serverCode_ << ", Validate transaction failed, serverCode: " << message_.serverCode_;
        return false;
    }

    return true;
}

void GameServerLoadIndicationTransaction::handleSelf()
{
    gameServersList_.updateGameServerLoad(message_.serverCode_, message_.load_);
}

}
}
}

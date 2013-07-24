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
    return gameServersList_.hasGameServer(message_.serverCode_);
}

void GameServerLoadIndicationTransaction::handleSelf()
{
    gameServersList_.updateGameServerLoad(message_.serverCode_, message_.load_);
}

}
}
}

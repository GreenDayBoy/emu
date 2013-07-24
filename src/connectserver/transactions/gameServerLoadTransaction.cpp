#include <connectserver/transactions/gameServerLoadTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServerLoadTransaction::GameServerLoadTransaction(const interface::GameServerLoadMessage &message, GameServersList &gameServersList):
    message_(message),
    gameServersList_(gameServersList) {}

bool GameServerLoadTransaction::validate() const
{
    return gameServersList_.hasGameServer(message_.serverCode_);
}

void GameServerLoadTransaction::handleSelf()
{
    gameServersList_.updateGameServerLoad(message_.serverCode_, message_.load_);
}

}
}
}

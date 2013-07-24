#include <connectserver/transactions/gameServersListResponseTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServersListResponseTransaction::GameServersListResponseTransaction(size_t hash,
                                                                       GameServersList::GameServersListContainer &servers,
                                                                       MessageSender &messageSender):
    hash_(hash),
    servers_(servers),
    messageSender_(messageSender) {}

bool GameServersListResponseTransaction::validate() const
{
    return true;
}

void GameServersListResponseTransaction::handleSelf()
{
    messageSender_.sendGameServersListResponse(hash_, servers_);
}

}
}
}

#include <glog/logging.h>
#include <connectserver/transactions/gameServersListResponseTransaction.hpp>
#include <interface/gameServersListResponse.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

GameServersListResponseTransaction::GameServersListResponseTransaction(size_t hash,
                                                                       const GameServersList::GameServersListContainer &servers,
                                                                       MessageSender &messageSender):
    hash_(hash),
    servers_(servers),
    messageSender_(messageSender) {}

bool GameServersListResponseTransaction::validate() const
{
    if(servers_.size() > interface::constants::kMaxGameServersListLength)
    {
        LOG(ERROR) << "hash: " << hash_ << ", Validate transaction failed, #servers: " << servers_.size();
        return false;
    }

    return true;
}

void GameServersListResponseTransaction::handleSelf()
{
    messageSender_.sendGameServersListResponse(hash_, servers_);
}

}
}
}

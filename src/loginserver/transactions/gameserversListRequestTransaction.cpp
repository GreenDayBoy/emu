#include <loginserver/transactions/gameserversListRequestTransaction.hpp>
#include <protocol/loginserver/gameserversListResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserversListRequestTransaction::GameserversListRequestTransaction(size_t hash,
                                                                     core::network::tcp::ConnectionsManager &connectionsManager,
                                                                     const GameserversList &gameserversList,
                                                                     const protocol::loginserver::GameserversListRequest &request):
    hash_(hash),
    connectionsManager_(connectionsManager),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserversListRequestTransaction::isValid() const
{
    return true;
}

void GameserversListRequestTransaction::handleValid()
{
    LOG(INFO) << "hash: " << hash_ << ", requested gameservers list.";
    protocol::loginserver::GameserversListResponse response(gameserversList_.getServers());

    connectionsManager_.send(hash_, response.getWriteStream().getPayload());
}

}
}
}

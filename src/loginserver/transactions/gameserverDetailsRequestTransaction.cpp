#include <loginserver/transactions/gameserverDetailsRequestTransaction.hpp>
#include <protocol/loginserver/gameserverDetailsResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserverDetailsRequestTransaction::GameserverDetailsRequestTransaction(size_t hash,
                                                                         core::network::tcp::ConnectionsManager &connectionsManager,
                                                                         const GameserversList &gameserversList,
                                                                         const protocol::loginserver::GameserverDetailsRequest &request):
    hash_(hash),
    connectionsManager_(connectionsManager),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserverDetailsRequestTransaction::isValid() const
{
    return gameserversList_.hasGameserver(request_.getGameserverCode());
}

void GameserverDetailsRequestTransaction::handleValid()
{
    LOG(INFO) << "hash: " << hash_ << ", requested gameserver details, gameserverCode: " << request_.getGameserverCode();
    protocol::loginserver::GameserverDetailsResponse response;

    connectionsManager_.send(hash_, response.getWriteStream().getPayload());
}

void GameserverDetailsRequestTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << hash_ << ", requested gameserver does not exists! gameserverCode: " << request_.getGameserverCode();

    connectionsManager_.disconnect(hash_);
}

}
}
}

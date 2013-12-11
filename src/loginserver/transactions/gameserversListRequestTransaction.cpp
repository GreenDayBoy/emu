#include <loginserver/transactions/gameserversListRequestTransaction.hpp>
#include <protocol/loginserver/gameserversListResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserversListRequestTransaction::GameserversListRequestTransaction(User &user,
                                                                     const GameserversList &gameserversList,
                                                                     const protocol::loginserver::GameserversListRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserversListRequestTransaction::isValid() const
{
    return true;
}

void GameserversListRequestTransaction::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash() << ", requested gameservers list.";
    protocol::loginserver::GameserversListResponse response(gameserversList_.getServers());

    user_.getConnection().send(response.getWriteStream().getPayload());
}

}
}
}

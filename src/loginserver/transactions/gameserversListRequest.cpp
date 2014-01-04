#include <loginserver/transactions/gameserversListRequest.hpp>
#include <streaming/loginserver/gameserversListResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserversListRequest::GameserversListRequest(User &user,
                                               const GameserversList &gameserversList,
                                               const streaming::loginserver::GameserversListRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserversListRequest::isValid() const
{
    return true;
}

void GameserversListRequest::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash();
    streaming::loginserver::GameserversListResponse response(gameserversList_.getServers());

    user_.getConnection().send(response.getWriteStream().getPayload());
}

}
}
}

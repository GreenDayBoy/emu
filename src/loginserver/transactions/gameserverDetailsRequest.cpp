#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <protocol/loginserver/gameserverDetailsResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserverDetailsRequest::GameserverDetailsRequest(User &user,
                                                   const GameserversList &gameserversList,
                                                   const protocol::loginserver::GameserverDetailsRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserverDetailsRequest::isValid() const
{
    return gameserversList_.hasGameserver(request_.getGameserverCode());
}

void GameserverDetailsRequest::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash() << ", requested gameserver details, gameserverCode: " << request_.getGameserverCode();

    const protocol::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(request_.getGameserverCode());

    protocol::loginserver::GameserverDetailsResponse response(gameserverInfo.address_, gameserverInfo.port_);
    user_.getConnection().send(response.getWriteStream().getPayload());
}

void GameserverDetailsRequest::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", requested gameserver does not exists! gameserverCode: " << request_.getGameserverCode();

    user_.getConnection().disconnect();
}

}
}
}

#include <loginserver/transactions/gameserverDetailsRequestTransaction.hpp>
#include <protocol/loginserver/gameserverDetailsResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserverDetailsRequestTransaction::GameserverDetailsRequestTransaction(User &user,
                                                                         const GameserversList &gameserversList,
                                                                         const protocol::loginserver::GameserverDetailsRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    request_(request) {}

bool GameserverDetailsRequestTransaction::isValid() const
{
    return gameserversList_.hasGameserver(request_.getGameserverCode());
}

void GameserverDetailsRequestTransaction::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash() << ", requested gameserver details, gameserverCode: " << request_.getGameserverCode();
    protocol::loginserver::GameserverDetailsResponse response;

    user_.getConnection().send(response.getWriteStream().getPayload());
}

void GameserverDetailsRequestTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", requested gameserver does not exists! gameserverCode: " << request_.getGameserverCode();

    user_.getConnection().disconnect();
}

}
}
}

#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserverDetailsRequest::GameserverDetailsRequest(User &user,
                                                   const GameserversList &gameserversList,
                                                   core::network::udp::Connection::Pointer gameserverConnection,
                                                   const streaming::loginserver::GameserverDetailsRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    gameserverConnection_(gameserverConnection),
    request_(request) {}

bool GameserverDetailsRequest::isValid() const
{
    return gameserversList_.hasGameserver(request_.getGameserverCode()) && gameserverConnection_ != nullptr;
}

void GameserverDetailsRequest::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash() << ", requested gameserver details, gameserverCode: " << request_.getGameserverCode();

    const streaming::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(request_.getGameserverCode());

    streaming::gameserver::RegisterUserRequest registerUserRequest(user_.getHash(), user_.getAccountId());
    gameserverConnection_->sendTo(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(gameserverInfo.address_),
                                                                 gameserverInfo.port_),
                                  registerUserRequest.getWriteStream().getPayload());

    streaming::loginserver::GameserverDetailsResponse gameserverDetailsResponse(gameserverInfo.address_, gameserverInfo.port_);
    user_.getConnection().send(gameserverDetailsResponse.getWriteStream().getPayload());
}

void GameserverDetailsRequest::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", requested gameserver does not exists! gameserverCode: " << request_.getGameserverCode();

    user_.getConnection().disconnect();
}

}
}
}

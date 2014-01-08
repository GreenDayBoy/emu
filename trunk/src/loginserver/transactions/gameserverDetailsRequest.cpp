#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

GameserverDetailsRequest::GameserverDetailsRequest(User &user,
                                                   GameserversList &gameserversList,
                                                   core::network::udp::Connection::Pointer udpConnection,
                                                   const streaming::loginserver::GameserverDetailsRequest &request):
    user_(user),
    gameserversList_(gameserversList),
    udpConnection_(udpConnection),
    request_(request) {}

bool GameserverDetailsRequest::isValid() const
{
    return gameserversList_.hasGameserver(request_.getGameserverCode()) && udpConnection_ != nullptr;
}

void GameserverDetailsRequest::handleValid()
{
    eMU_LOG(info) << "hash: " << user_.getHash() << ", gameserverCode: " << request_.getGameserverCode();

    const streaming::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(request_.getGameserverCode());

    streaming::gameserver::RegisterUserRequest registerUserRequest({user_.getHash(), user_.getAccountId()});
    udpConnection_->sendTo(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(gameserverInfo.address_),
                                                          gameserverInfo.port_),
                           registerUserRequest.getWriteStream().getPayload());
}

void GameserverDetailsRequest::handleInvalid()
{
    eMU_LOG(error) << "hash: " << user_.getHash() << ", requested gameserver does not exists! gameserverCode: " << request_.getGameserverCode();

    user_.getConnection().disconnect();
}

}
}
}

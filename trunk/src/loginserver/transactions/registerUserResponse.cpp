#include <loginserver/transactions/registerUserResponse.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

RegisterUserResponse::RegisterUserResponse(core::common::Factory<User> &usersFactory,
                                           GameserversList &gameserversList,
                                           const streaming::gameserver::RegisterUserResponse &response):
    usersFactory_(usersFactory),
    gameserversList_(gameserversList),
    response_(response) {}

bool RegisterUserResponse::isValid() const
{
    return usersFactory_.exists(response_.getUserHash()) && gameserversList_.hasGameserver(response_.getGameserverCode());
}

void RegisterUserResponse::handleValid()
{
    LOG(INFO) << "hash: " << response_.getUserHash() << ", registration result: " << static_cast<uint32_t>(response_.getResult());

    User &user = usersFactory_.find(response_.getUserHash());

    if(response_.getResult() == streaming::gameserver::UserRegistrationResult::Failed)
    {
        LOG(INFO) << "hash: " << user.getHash() << ", registration to gameserver failed. Disconnecting.";
        user.getConnection().disconnect();
    }
    else
    {
        LOG(INFO) << "hash: " << user.getHash() << ", registered to gameserver, code: " << response_.getGameserverCode();

        const streaming::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(response_.getGameserverCode());

        LOG(INFO) << "hash: " << user.getHash() << ", sending gameserver details response, address: " << gameserverInfo.address_
                  << ", port: " << gameserverInfo.port_;

        streaming::loginserver::GameserverDetailsResponse gameserverDetailsResponse(gameserverInfo.address_, gameserverInfo.port_);
        user.getConnection().send(gameserverDetailsResponse.getWriteStream().getPayload());
    }
}

void RegisterUserResponse::handleInvalid()
{
    LOG(ERROR) << "Received invalid user registration data, hash: " << response_.getUserHash()
               << ", gameserverCode: " << response_.getGameserverCode();
}

}
}
}

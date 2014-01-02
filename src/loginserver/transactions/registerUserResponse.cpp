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
    bool result = true;

    try
    {
        usersFactory_.find(response_.getUserHash());
        result = gameserversList_.hasGameserver(response_.getGameserverCode());
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        result = false;
    }

    return result;
}

void RegisterUserResponse::handleValid()
{
    LOG(INFO) << "hash: " << response_.getUserHash() << ", registration result: " << static_cast<uint32_t>(response_.getResult());

    User &user = usersFactory_.find(response_.getUserHash());

    if(response_.getResult() == streaming::gameserver::UserRegistrationResult::Failed)
    {
        user.getConnection().disconnect();
    }
    else
    {
        const streaming::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(response_.getGameserverCode());

        streaming::loginserver::GameserverDetailsResponse gameserverDetailsResponse(gameserverInfo.address_, gameserverInfo.port_);
        user.getConnection().send(gameserverDetailsResponse.getWriteStream().getPayload());
    }
}

void RegisterUserResponse::handleInvalid()
{
    LOG(ERROR) << "Received invalid registration data, hash: " << response_.getUserHash()
               << ", gameserverCode: " << response_.getGameserverCode();
}

}
}
}

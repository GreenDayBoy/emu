#include <loginserver/transactions/registerUserResponse.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>

#include <core/common/logging.hpp>

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
    eMU_LOG(info) << "hash: " << response_.getUserHash() << ", registration result: " << static_cast<uint32_t>(response_.getResult());

    User &user = usersFactory_.find(response_.getUserHash());

    if(response_.getResult() == streaming::gameserver::UserRegistrationResult::Failed)
    {
        eMU_LOG(info) << "hash: " << user.getHash() << ", registration to gameserver failed. Disconnecting.";
        user.getConnection().disconnect();
    }
    else
    {
        eMU_LOG(info) << "hash: " << user.getHash() << ", registered to gameserver, code: " << response_.getGameserverCode();

        const streaming::loginserver::GameserverInfo &gameserverInfo = gameserversList_.getGameserverInfo(response_.getGameserverCode());

        eMU_LOG(info) << "hash: " << user.getHash() << ", sending gameserver details response, address: " << gameserverInfo.address_
            << ", port: " << gameserverInfo.port_;

        streaming::loginserver::GameserverDetailsResponse gameserverDetailsResponse(gameserverInfo.address_, gameserverInfo.port_);
        user.getConnection().send(gameserverDetailsResponse.getWriteStream().getPayload());
    }
}

void RegisterUserResponse::handleInvalid()
{
    eMU_LOG(error) << "Received invalid user registration data, hash: " << response_.getUserHash()
        << ", gameserverCode: " << response_.getGameserverCode();
}

}
}
}

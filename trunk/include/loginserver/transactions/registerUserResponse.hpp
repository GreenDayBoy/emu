#pragma once

#include <core/common/transaction.hpp>
#include <core/common/factory.hpp>
#include <loginserver/user.hpp>
#include <loginserver/gameserversList.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class RegisterUserResponse: public core::common::Transaction
{
public:
    RegisterUserResponse(core::common::Factory<User> &usersFactory,
                         GameserversList &gameserversList,
                         const streaming::gameserver::RegisterUserResponse &response);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    GameserversList &gameserversList_;
    streaming::gameserver::RegisterUserResponse response_;
};

}
}
}

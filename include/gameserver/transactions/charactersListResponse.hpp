#pragma once

#include <core/common/factory.hpp>
#include <core/common/transaction.hpp>
#include <gameserver/user.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class CharactersListResponse: public core::common::Transaction
{
public:
    CharactersListResponse(core::common::Factory<User> &usersFactory,
                           const streaming::dataserver::CharactersListResponse &response);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    streaming::dataserver::CharactersListResponse response_;
};

}
}
}

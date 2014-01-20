#pragma once

#include <core/common/factory.hpp>
#include <core/common/transaction.hpp>
#include <gameserver/user.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class CharacterCreateResponse: public core::common::Transaction
{
public:
    CharacterCreateResponse(core::common::Factory<User> &usersFactory,
                            const streaming::dataserver::CharacterCreateResponse &response);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    streaming::dataserver::CharacterCreateResponse response_;
};

}
}
}

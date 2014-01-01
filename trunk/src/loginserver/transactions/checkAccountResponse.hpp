#pragma once

#include <core/common/factory.hpp>
#include <core/transactions/transaction.hpp>
#include <loginserver/user.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class CheckAccountResponse: public core::transactions::Transaction
{
public:
    CheckAccountResponse(core::common::Factory<User> &usersFactory,
                         const streaming::dataserver::CheckAccountResponse &response);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    streaming::dataserver::CheckAccountResponse response_;
};

}
}
}

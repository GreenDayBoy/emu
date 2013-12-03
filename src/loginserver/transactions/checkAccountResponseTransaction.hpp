#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <core/common/usersFactory.hpp>

#include <loginserver/user.hpp>

#include <protocol/dataserver/checkAccountResponse.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class CheckAccountResponseTransaction: public core::transactions::Transaction
{
public:
    CheckAccountResponseTransaction(core::network::tcp::ConnectionsManager &connectionsManager,
                                    core::common::UsersFactory<User> &usersFactory,
                                    const protocol::dataserver::CheckAccountResponse &response);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::network::tcp::ConnectionsManager &connectionsManager_;
    core::common::UsersFactory<User> &usersFactory_;
    protocol::dataserver::CheckAccountResponse response_;
};

}
}
}

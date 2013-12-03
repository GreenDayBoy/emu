#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connection.hpp>
#include <core/network/tcp/connectionsManager.hpp>

#include <loginserver/user.hpp>
#include <protocol/loginserver/loginRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class LoginRequestTransaction: public core::transactions::Transaction
{
public:
    LoginRequestTransaction(User &user,
                            core::network::tcp::ConnectionsManager &connectionsManager,
                            core::network::tcp::Connection &dataserverConnection,
                            const protocol::loginserver::LoginRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    core::network::tcp::Connection &dataserverConnection_;
    protocol::loginserver::LoginRequest request_;
};

}
}
}

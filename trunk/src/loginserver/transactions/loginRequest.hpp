#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connection.hpp>

#include <loginserver/user.hpp>
#include <streaming/loginserver/loginRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class LoginRequest: public core::transactions::Transaction
{
public:
    LoginRequest(User &user,
                 core::network::tcp::Connection::Pointer dataserverConnection,
                 const streaming::loginserver::LoginRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    core::network::tcp::Connection::Pointer dataserverConnection_;
    streaming::loginserver::LoginRequest request_;
};

}
}
}

#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connection.hpp>
#include <core/network/tcp/connectionsManager.hpp>

#include <protocol/loginserver/decoders/loginRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class LoginRequestTransaction: public eMU::core::transactions::Transaction
{
public:
    LoginRequestTransaction(size_t hash,
                            core::network::tcp::ConnectionsManager &connectionsManager,
                            core::network::tcp::Connection &dataserverConnection,
                            const protocol::loginserver::decoders::LoginRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    size_t hash_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    core::network::tcp::Connection &dataserverConnection_;
    protocol::loginserver::decoders::LoginRequest request_;
};

}
}
}

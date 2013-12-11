#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <loginserver/gameserversList.hpp>

#include <protocol/loginserver/gameserverDetailsRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class GameserverDetailsRequestTransaction: public core::transactions::Transaction
{
public:
    GameserverDetailsRequestTransaction(size_t hash,
                                        core::network::tcp::ConnectionsManager &connectionsManager,
                                        const GameserversList &gameserversList,
                                        const protocol::loginserver::GameserverDetailsRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    size_t hash_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    const GameserversList &gameserversList_;
    protocol::loginserver::GameserverDetailsRequest request_;
};

}
}
}

#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <loginserver/gameserversList.hpp>

#include <protocol/loginserver/gameserversListRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class GameserversListRequestTransaction: public core::transactions::Transaction
{
public:
    GameserversListRequestTransaction(size_t hash,
                            core::network::tcp::ConnectionsManager &connectionsManager,
                            const GameserversList &gameserversList,
                            const protocol::loginserver::GameserversListRequest &request);

private:
    bool isValid() const;
    void handleValid();

    size_t hash_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    const GameserversList &gameserversList_;
    protocol::loginserver::GameserversListRequest request_;
};

}
}
}

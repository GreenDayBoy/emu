#pragma once

#include <core/transactions/transaction.hpp>
#include <connectserver/gameServersList.hpp>
#include <connectserver/messageSender.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

class GameServersListResponseTransaction: public eMU::core::transactions::Transaction
{
public:
    GameServersListResponseTransaction(size_t hash, GameServersList::GameServersListContainer &servers, MessageSender &messageSender);

private:
    bool validate() const;
    void handleSelf();

    size_t hash_;
    GameServersList::GameServersListContainer &servers_;
    MessageSender &messageSender_;
};

}
}
}

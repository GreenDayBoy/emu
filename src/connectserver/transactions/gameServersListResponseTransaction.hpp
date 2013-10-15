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
    GameServersListResponseTransaction(size_t hash, const GameServersList::GameServersListContainer &servers, MessageSender &messageSender);

    bool isValid() const;
    void handle();

private:
    size_t hash_;
    const GameServersList::GameServersListContainer &servers_;
    MessageSender &messageSender_;
};

}
}
}

#pragma once

#include <core/transactions/transaction.hpp>
#include <connectserver/gameServersList.hpp>
#include <interface/gameServerLoadMessage.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

class GameServerLoadTransaction: public eMU::core::transactions::Transaction
{
public:
    GameServerLoadTransaction(const interface::GameServerLoadMessage &message, GameServersList &gameServersList);

private:
    bool validate() const;
    void handleSelf();

    interface::GameServerLoadMessage message_;
    GameServersList &gameServersList_;
};

}
}
}

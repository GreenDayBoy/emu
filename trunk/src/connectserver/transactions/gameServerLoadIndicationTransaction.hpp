#pragma once

#include <core/transactions/transaction.hpp>
#include <connectserver/gameServersList.hpp>
#include <interface/gameServerLoadIndication.hpp>

namespace eMU
{
namespace connectserver
{ namespace transactions
{

class GameServerLoadIndicationTransaction: public eMU::core::transactions::Transaction
{
public:
    GameServerLoadIndicationTransaction(const interface::GameServerLoadIndication &message, GameServersList &gameServersList);

private:
    bool validate() const;
    void handleSelf();

    interface::GameServerLoadIndication message_;
    GameServersList &gameServersList_;
};

}
}
}

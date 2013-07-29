#pragma once

#include <connectserver/messageSender.hpp>
#include <core/transactions/manager.hpp>
#include <connectserver/gameServersList.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

class GameServerAddressResponseTransaction: public eMU::core::transactions::Transaction
{
public:
    GameServerAddressResponseTransaction(size_t hash,
                                         MessageSender &messageSender,
                                         const GameServersList &gameServersList,
                                         uint16_t code);

private:
    bool validate() const;
    void handleSelf();

    size_t hash_;
    MessageSender &messageSender_;
    const GameServersList &gameServersList_;
    uint16_t code_;
};

}
}
}

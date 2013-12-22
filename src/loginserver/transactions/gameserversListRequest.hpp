#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/gameserversList.hpp>
#include <loginserver/user.hpp>

#include <protocol/loginserver/gameserversListRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class GameserversListRequest: public core::transactions::Transaction
{
public:
    GameserversListRequest(User &user,
                           const GameserversList &gameserversList,
                           const protocol::loginserver::GameserversListRequest &request);

private:
    bool isValid() const;
    void handleValid();

    User &user_;
    const GameserversList &gameserversList_;
    protocol::loginserver::GameserversListRequest request_;
};

}
}
}

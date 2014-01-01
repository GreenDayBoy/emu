#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/gameserversList.hpp>
#include <loginserver/user.hpp>

#include <streaming/loginserver/gameserversListRequest.hpp>

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
                           const streaming::loginserver::GameserversListRequest &request);

private:
    bool isValid() const;
    void handleValid();

    User &user_;
    const GameserversList &gameserversList_;
    streaming::loginserver::GameserversListRequest request_;
};

}
}
}

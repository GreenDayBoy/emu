#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/gameserversList.hpp>
#include <loginserver/user.hpp>

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
    GameserverDetailsRequestTransaction(User &user,
                                        const GameserversList &gameserversList,
                                        const protocol::loginserver::GameserverDetailsRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    const GameserversList &gameserversList_;
    protocol::loginserver::GameserverDetailsRequest request_;
};

}
}
}

#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/gameserversList.hpp>
#include <loginserver/user.hpp>

#include <streaming/loginserver/gameserverDetailsRequest.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class GameserverDetailsRequest: public core::transactions::Transaction
{
public:
    GameserverDetailsRequest(User &user,
                             const GameserversList &gameserversList,
                             const streaming::loginserver::GameserverDetailsRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    const GameserversList &gameserversList_;
    streaming::loginserver::GameserverDetailsRequest request_;
};

}
}
}

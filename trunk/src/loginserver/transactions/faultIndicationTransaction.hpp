#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/user.hpp>
#include <protocol/dataserver/faultIndication.hpp>
#include <core/common/factory.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class FaultIndicationTransaction: public core::transactions::Transaction
{
public:
    FaultIndicationTransaction(core::common::Factory<User> &usersFactory,
                               const protocol::dataserver::FaultIndication &indication);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    protocol::dataserver::FaultIndication indication_;
};

}
}
}

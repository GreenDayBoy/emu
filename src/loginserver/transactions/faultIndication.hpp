#pragma once

#include <core/transactions/transaction.hpp>
#include <loginserver/user.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <core/common/factory.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class FaultIndication: public core::transactions::Transaction
{
public:
    FaultIndication(core::common::Factory<User> &usersFactory,
                    const streaming::dataserver::FaultIndication &indication);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::common::Factory<User> &usersFactory_;
    streaming::dataserver::FaultIndication indication_;
};

}
}
}

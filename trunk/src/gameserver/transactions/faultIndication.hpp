#pragma once

#include <core/common/transaction.hpp>
#include <gameserver/user.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <core/common/factory.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class FaultIndication: public core::common::Transaction
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

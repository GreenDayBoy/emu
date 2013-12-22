#include <loginserver/transactions/faultIndication.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

FaultIndication::FaultIndication(core::common::Factory<User> &usersFactory,
                                 const protocol::dataserver::FaultIndication &indication):
    usersFactory_(usersFactory),
    indication_(indication) {}

bool FaultIndication::isValid() const
{
    bool result = true;

    try
    {
        usersFactory_.find(indication_.getClientHash());
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        result = false;
    }

    return result;
}

void FaultIndication::handleValid()
{
    User &user = usersFactory_.find(indication_.getClientHash());
    LOG(ERROR) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId() << ", fault from dataserver received! message: " << indication_.getMessage();
    user.getConnection().disconnect();
}

void FaultIndication::handleInvalid()
{
    LOG(ERROR) << "hash: " << indication_.getClientHash() << " given in dataserver fault indication does not exist!";
}

}
}
}

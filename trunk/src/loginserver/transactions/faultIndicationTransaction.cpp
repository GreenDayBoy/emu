#include <loginserver/transactions/faultIndicationTransaction.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

FaultIndicationTransaction::FaultIndicationTransaction(core::common::Factory<User> &usersFactory,
                                                       const protocol::dataserver::FaultIndication &indication):
    usersFactory_(usersFactory),
    indication_(indication) {}

bool FaultIndicationTransaction::isValid() const
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

void FaultIndicationTransaction::handleValid()
{
    User &user = usersFactory_.find(indication_.getClientHash());
    LOG(ERROR) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId() << ", fault from dataserver received! message: " << indication_.getMessage();
    user.getConnection().disconnect();
}

void FaultIndicationTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << indication_.getClientHash() << " given in dataserver fault indication does not exist!";
}

}
}
}

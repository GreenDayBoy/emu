#include <loginserver/transactions/faultIndicationTransaction.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

FaultIndicationTransaction::FaultIndicationTransaction(core::network::tcp::ConnectionsManager &connectionsManager,
                                                       core::common::UsersFactory<User> &usersFactory,
                                                       const protocol::dataserver::decoders::FaultIndication &indication):
    connectionsManager_(connectionsManager),
    usersFactory_(usersFactory),
    indication_(indication) {}

bool FaultIndicationTransaction::isValid() const
{
    return usersFactory_.exists(indication_.getClientHash());
}

void FaultIndicationTransaction::handleValid()
{
    User &user = usersFactory_.find(indication_.getClientHash());
    LOG(ERROR) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId() << ", fault from dataserver received! message: " << indication_.getMessage();
    connectionsManager_.disconnect(user.getHash());
}

void FaultIndicationTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << indication_.getClientHash() << " given in dataserver fault indication does not exist!";
}

}
}
}

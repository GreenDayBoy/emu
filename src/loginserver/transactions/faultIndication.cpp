#include <loginserver/transactions/faultIndication.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

FaultIndication::FaultIndication(core::common::Factory<User> &usersFactory,
                                 const streaming::dataserver::FaultIndication &indication):
    usersFactory_(usersFactory),
    indication_(indication) {}

bool FaultIndication::isValid() const
{
    return usersFactory_.exists(indication_.getUserHash());
}

void FaultIndication::handleValid()
{
    User &user = usersFactory_.find(indication_.getUserHash());

    eMU_LOG(error) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId() << ", message: " << indication_.getMessage();
    user.getConnection().disconnect();
}

void FaultIndication::handleInvalid()
{
    eMU_LOG(error) << "hash: " << indication_.getUserHash() << ", given in dataserver fault indication does not exist!";
}

}
}
}

#include <dataserver/transactions/databaseTransaction.hpp>
#include <streaming/dataserver/faultIndication.hpp>

#include <sstream>
#include <glog/logging.h>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

DatabaseTransaction::DatabaseTransaction(User &user,
                                         database::SqlInterface &sqlInterface,
                                         core::network::tcp::NetworkUser::Hash userHash):
    user_(user),
    sqlInterface_(sqlInterface),
    userHash_(userHash) {}

bool DatabaseTransaction::isValid() const
{
    return sqlInterface_.isAlive();
}

void DatabaseTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash()
               << ", Connection to database is died.";

    this->sendFaultIndication("Connection to database is died");
}

void DatabaseTransaction::handleValid()
{

}

void DatabaseTransaction::sendFaultIndication(const std::string &message)
{
    streaming::dataserver::FaultIndication indication(userHash_, message);
    user_.getConnection().send(indication.getWriteStream().getPayload());
}

}
}
}

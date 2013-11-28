#include <dataserver/transactions/checkAccountRequestTransaction.hpp>
#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/dataserver/encoders/faultIndication.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>

#include <sstream>
#include <glog/logging.h>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequestTransaction::CheckAccountRequestTransaction(size_t hash,
                                                               database::SqlInterface &sqlInterface,
                                                               core::network::tcp::ConnectionsManager &connectionsManager,
                                                               const protocol::dataserver::decoders::CheckAccountRequest &request):
    hash_(hash),
    sqlInterface_(sqlInterface),
    connectionsManager_(connectionsManager),
    request_(request) {}

bool CheckAccountRequestTransaction::isValid() const
{
    if(sqlInterface_.isAlive())
    {
        return true;
    }
    else
    {
        LOG(ERROR) << "hash: " << hash_ << ", Connection to database is died!";
        return false;
    }
}

void CheckAccountRequestTransaction::handleValid()
{
    std::stringstream query;
    query << "SELECT"
          << " `eMU_AccountCheck`(" << request_.getAccountId()
                                    << ", "
                                    << request_.getPassword()
                                    << ");";

    if(sqlInterface_.executeQuery(query.str()))
    {
        const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();

        if(queryResult.getRows().size() > 0)
        {
            protocol::dataserver::CheckAccountResult result = static_cast<protocol::dataserver::CheckAccountResult>(queryResult.getRows()[0].getValue<uint32_t>(0));
            protocol::dataserver::encoders::CheckAccountResponse response(request_.getClientHash(), result);

            connectionsManager_.send(hash_, response.getWriteStream().getPayload());
        }
        else
        {
            LOG(ERROR) << "Empty query results for accountId: " << request_.getAccountId();
            this->sendFaultIndication("Check account query result is empty");
        }
    }
    else
    {
        this->sendFaultIndication(sqlInterface_.getErrorMessage());
    }
}

void CheckAccountRequestTransaction::handleInvalid()
{
    this->sendFaultIndication("Connection to database is died");
}

void CheckAccountRequestTransaction::sendFaultIndication(const std::string &message)
{
    protocol::dataserver::encoders::FaultIndication indication(request_.getClientHash(), message);
    connectionsManager_.send(hash_, indication.getWriteStream().getPayload());
}

}
}
}

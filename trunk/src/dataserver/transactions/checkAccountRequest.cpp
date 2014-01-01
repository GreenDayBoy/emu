#include <dataserver/transactions/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>

#include <sstream>
#include <glog/logging.h>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequest::CheckAccountRequest(User &user,
                                         database::SqlInterface &sqlInterface,
                                         const streaming::dataserver::CheckAccountRequest &request):
    user_(user),
    sqlInterface_(sqlInterface),
    request_(request) {}

bool CheckAccountRequest::isValid() const
{
    return sqlInterface_.isAlive();
}

void CheckAccountRequest::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash() << ", clientHash: " << request_.getClientHash() << ", checking account: " << request_.getAccountId();

    std::stringstream query;
    query << "SELECT"
          << " `eMU_AccountCheck`("
          << "'" << request_.getAccountId() << "'"
          << ", '" << request_.getPassword() << "'"
          << ", '" << "127.0.0.1" << "');";

    if(sqlInterface_.executeQuery(query.str()))
    {
        const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();

        if(queryResult.getRows().size() > 0)
        {
            streaming::dataserver::CheckAccountResult result = static_cast<streaming::dataserver::CheckAccountResult>(queryResult.getRows()[0].getValue<uint32_t>(0));
            streaming::dataserver::CheckAccountResponse response(request_.getClientHash(), result);

            user_.getConnection().send(response.getWriteStream().getPayload());
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

void CheckAccountRequest::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", Connection to database is died!";

    this->sendFaultIndication("Connection to database is died");
}

void CheckAccountRequest::sendFaultIndication(const std::string &message)
{
    streaming::dataserver::FaultIndication indication(request_.getClientHash(), message);
    user_.getConnection().send(indication.getWriteStream().getPayload());
}

}
}
}

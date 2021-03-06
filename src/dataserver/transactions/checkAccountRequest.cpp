#include <dataserver/transactions/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>

#include <sstream>
#include <core/common/logging.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequest::CheckAccountRequest(User &user,
                                         database::SqlInterface &sqlInterface,
                                         const streaming::dataserver::CheckAccountRequest &request):
    DatabaseTransaction(user, sqlInterface, request.getUserHash()),
    request_(request) {}

void CheckAccountRequest::handleValid()
{
    eMU_LOG(info) << "hash: " << user_.getHash()
        << ", userHash: " << request_.getUserHash()
        << ", accountId: " << request_.getAccountId();

    std::stringstream query;
    query << "SELECT"
        << " `eMU_AccountCheck`("
        << "'" << request_.getAccountId() << "'"
        << ", '" << request_.getPassword() << "'"
        << ", '" << "127.0.0.1" << "');";

    if(!sqlInterface_.executeQuery(query.str()))
    {
        this->sendFaultIndication(sqlInterface_.getErrorMessage());
        return;
    }

    const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();

    if(queryResult.getRows().empty())
    {
        eMU_LOG(error) << "Empty query results for accountId: " << request_.getAccountId();
        this->sendFaultIndication("Check account query result is empty");

        return;
    }

    streaming::dataserver::CheckAccountResult result = static_cast<streaming::dataserver::CheckAccountResult>(queryResult.getRows()[0].getValue<uint32_t>(0));
    streaming::dataserver::CheckAccountResponse response(request_.getUserHash(), result);

    user_.getConnection().send(response.getWriteStream().getPayload());
}

}
}
}

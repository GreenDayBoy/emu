#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

#include <sstream>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequestTransaction::CheckAccountRequestTransaction(const core::network::Payload &packet, database::SqlInterface &sqlInterface):
    packet_(packet),
    sqlInterface_(sqlInterface) {}

bool CheckAccountRequestTransaction::isValid() const
{
    return true;
}

void CheckAccountRequestTransaction::handle()
{
    std::stringstream query;
    query << "SELECT"
          << " `eMU_AccountCheck`();";

    sqlInterface_.executeQuery(query.str());

    const database::QueryResult&& queryResult = sqlInterface_.fetchQueryResult();

    if(queryResult.getRows().size() > 0)
    {
        int32_t checkResult = queryResult.getFieldValue<int32_t>(0);
    }
}

}
}
}

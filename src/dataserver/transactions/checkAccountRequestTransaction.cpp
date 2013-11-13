#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequestTransaction::CheckAccountRequestTransaction(database::Database &database):
    database_(database) {}

bool CheckAccountRequestTransaction::isValid() const
{
    return true;
}

void CheckAccountRequestTransaction::handle()
{
    std::stringstream query;
    query << "SELECT"
          << " `eMU_AccountCheck`();";

    database_.execute(query.str());

    const database::QueryResult&& queryResult = database_.fetchQueryResult();

    if(queryResult.getRows().size() > 0)
    {
        int32_t checkResult = queryResult.getFieldValue<int32_t>(0);
    }
}

}
}
}

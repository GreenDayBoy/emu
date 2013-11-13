#include <dataserver/database/database.hpp>

#include <glog/logging.h>
#include <boost/algorithm/string/replace.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

Database::Database(SqlInterface &sqlInterface):
    sqlInterface_(sqlInterface) {}

void Database::startup(const std::string &hostname, uint16_t port, const std::string &userName,
                       const std::string &password, const std::string &databaseName)
{
    if(!sqlInterface_.initialize())
    {
        throw InitializationFailedException();
    }

    if(!sqlInterface_.connect(hostname, port, userName, password, databaseName))
    {
        throw ConnectionFailedException();
    }
}

void Database::cleanup()
{
    sqlInterface_.cleanup();
}

void Database::execute(std::string query)
{
    boost::algorithm::replace_all(query, "'", "\\'");
    boost::algorithm::replace_all(query, "\\", "\\\\");

    if(sqlInterface_.executeQuery(query))
    {
        LOG(INFO) << "Executed query: " << query;
    }
    else
    {
        LOG(ERROR) << "Execute query failed. Reason: " << sqlInterface_.getErrorMessage() << ", query: " << query;
    }
}

const QueryResult&& Database::fetchQueryResult()
{
    return sqlInterface_.fetchQueryResult();
}

}
}
}

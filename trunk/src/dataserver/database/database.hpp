#pragma once

#include <core/common/exception.hpp>
#include <dataserver/database/sqlInterface.hpp>

#include <sstream>

namespace eMU
{
namespace dataserver
{
namespace database
{

class Database: boost::noncopyable
{
public:
    class InitializationFailedException: public core::common::Exception {};
    class ConnectionFailedException: public core::common::Exception {};

    Database(SqlInterface &sqlInterface);

    void startup(const std::string &hostname, uint16_t port,
                 const std::string &userName,  const std::string &password, const std::string &databaseName);
    void cleanup();
    void execute(std::string query);

    const QueryResult&& fetchQueryResult();

private:
    SqlInterface &sqlInterface_;
};

}
}
}

#pragma once

#include <dataserver/database/queryResult.hpp>

#include <string>
#include <boost/noncopyable.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

class SqlInterface: boost::noncopyable
{
public:
    virtual ~SqlInterface();

    virtual bool initialize() = 0;
    virtual bool connect(const std::string &hostname, uint16_t port, const std::string &userName,
                         const std::string &password, const std::string &databaseName) = 0;

    virtual void cleanup() = 0;
    virtual std::string getErrorMessage() = 0;

    virtual bool executeQuery(const std::string &query) = 0;
    virtual const QueryResult&& fetchQueryResult() = 0;
};

}
}
}

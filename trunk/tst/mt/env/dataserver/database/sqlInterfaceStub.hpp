#pragma once

#include <dataserver/database/sqlInterface.hpp>

#include <gmock/gmock.h>
#include <queue>

namespace eMU
{
namespace mt
{
namespace env
{
namespace dataserver
{
namespace database
{

class SqlInterfaceStub: public eMU::dataserver::database::SqlInterface
{
public:
    struct Query
    {
        bool queryStatus_;
        eMU::dataserver::database::QueryResult queryResult_;
    };

    class UnexpectedQueryExecutionException {};
    class UnexpectedFetchResultExecutionException {};

    typedef std::queue<bool> QueryStatusContainer;
    typedef std::queue<eMU::dataserver::database::QueryResult> QueryResultContainer;

    bool initialize();
    bool connect(const std::string &hostname, uint16_t port, const std::string &userName,
                 const std::string &password, const std::string &databaseName);

    void cleanup();
    std::string getErrorMessage();

    bool executeQuery(std::string query);
    eMU::dataserver::database::QueryResult fetchQueryResult();

    void pushQueryStatus(bool status);
    void pushQueryResult(const eMU::dataserver::database::QueryResult &queryResult);

private:
    QueryStatusContainer queriesStatus_;
    QueryResultContainer queriesResult_;
};

}
}
}
}
}

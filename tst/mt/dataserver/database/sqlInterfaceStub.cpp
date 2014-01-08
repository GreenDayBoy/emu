#include <mt/dataserver/database/sqlInterfaceStub.hpp>

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

SqlInterfaceStub::SqlInterfaceStub():
    isAlive_(true) {}

bool SqlInterfaceStub::initialize() { return true; }

bool SqlInterfaceStub::connect(const std::string &hostname, uint16_t port, const std::string &userName,
                               const std::string &password, const std::string &databaseName) { return true; }

void SqlInterfaceStub::cleanup() {}
std::string SqlInterfaceStub::getErrorMessage() { return ""; }

bool SqlInterfaceStub::executeQuery(std::string query)
{
    EXPECT_FALSE(queriesStatus_.empty()) << "Unexpected database query execution!";

    bool status = queriesStatus_.front();
    queriesStatus_.pop();

    return status;
}

eMU::dataserver::database::QueryResult SqlInterfaceStub::fetchQueryResult()
{
    EXPECT_FALSE(queriesResult_.empty()) << "No results to fetch!";

    eMU::dataserver::database::QueryResult result = queriesResult_.front();
    queriesResult_.pop();

    return result;
}

bool SqlInterfaceStub::isAlive()
{
    return isAlive_;
}

void SqlInterfaceStub::setAlive()
{
    isAlive_ = true;
}

void SqlInterfaceStub::setDied()
{
    isAlive_ = false;
}

void SqlInterfaceStub::pushQueryStatus(bool status)
{
    queriesStatus_.push(status);
}

void SqlInterfaceStub::pushQueryResult(const eMU::dataserver::database::QueryResult &queryResult)
{
    queriesResult_.push(queryResult);
}


}
}
}
}
}


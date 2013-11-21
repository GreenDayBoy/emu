#pragma once

#include <dataserver/database/sqlInterface.hpp>

#include <gmock/gmock.h>

namespace eMU
{
namespace ut
{
namespace env
{
namespace dataserver
{
namespace database
{

class SqlInterfaceMock: public eMU::dataserver::database::SqlInterface
{
public:
    MOCK_METHOD0(initialize, bool());
    MOCK_METHOD5(connect, bool(const std::string &hostname, uint16_t port, const std::string &userName,
                               const std::string &password, const std::string &databaseName));
    MOCK_METHOD0(cleanup, void());
    MOCK_METHOD0(getErrorMessage, std::string());
    MOCK_METHOD1(executeQuery, void(std::string query));
    MOCK_METHOD0(fetchQueryResult, eMU::dataserver::database::QueryResult());
};

}
}
}
}
}

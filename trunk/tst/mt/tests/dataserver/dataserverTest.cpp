#include <dataserver/server.hpp>
#include <mt/env/dataserver/database/sqlInterfaceStub.hpp>
#include <mt/env/testExceptionsCatch.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/encoders/checkAccountRequest.hpp>
#include <protocol/dataserver/decoders/checkAccountResponse.hpp>
#include <protocol/dataserver/decoders/faultIndication.hpp>

#include <gtest/gtest.h>

using eMU::dataserver::Server;
using eMU::dataserver::database::QueryResult;
using eMU::dataserver::database::Row;

using eMU::mt::env::asioStub::io_service;
using eMU::mt::env::dataserver::database::SqlInterfaceStub;

using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::encoders::CheckAccountRequest;
using eMU::protocol::dataserver::decoders::CheckAccountResponse;
using eMU::protocol::dataserver::decoders::FaultIndication;
namespace MessageIds = eMU::protocol::dataserver::MessageIds;

class DataserverTest: public ::testing::Test
{
protected:
    DataserverTest()
    {
        prepareConfiguration();
    }

    void prepareConfiguration()
    {
        configuration_.maxNumberOfUsers_ = 5;
        configuration_.port_ = 55960;
    }

    io_service ioService_;
    SqlInterfaceStub sqlInterface_;
    Server::Configuration configuration_;
};

TEST_F(DataserverTest, CheckAccountShoulBeSuccesful)
{
    Server server(ioService_, sqlInterface_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.estabilishTcpConnection();

    size_t clientHash = 0x1234;
    CheckAccountResult checkAccountResult = CheckAccountResult::AcoountInUse;

    QueryResult queryResult;
    Row &row = queryResult.createRow(Row::Fields());
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(checkAccountResult)));

    sqlInterface_.pushQueryResult(queryResult);
    sqlInterface_.pushQueryStatus(true);

    CheckAccountRequest request(clientHash, "Account", "Password");   
    IO_CHECK(ioService_.send(connectionHash, request.getWriteStream()));

    const ReadStream &readStream = ioService_.receive(connectionHash);
    ASSERT_EQ(MessageIds::kCheckAccountResponse, readStream.getId());

    CheckAccountResponse response(readStream);
    ASSERT_EQ(clientHash, response.getClientHash());
    ASSERT_EQ(checkAccountResult, response.getResult());

    IO_CHECK(ioService_.disconnect(connectionHash));
}

TEST_F(DataserverTest, WhenQueryExecutionWasFailedThenFaultIndicationShouldBeReceived)
{
    Server server(ioService_, sqlInterface_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.estabilishTcpConnection();

    size_t clientHash = 0x1234;
    sqlInterface_.pushQueryStatus(false);

    CheckAccountRequest request(clientHash, "Account", "Password");
    IO_CHECK(ioService_.send(connectionHash, request.getWriteStream()));

    const ReadStream &readStream = ioService_.receive(connectionHash);
    ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());

    FaultIndication indication(readStream);
    ASSERT_EQ(clientHash, indication.getClientHash());

    IO_CHECK(ioService_.disconnect(connectionHash));
}

TEST_F(DataserverTest, WhenQueryResultIsEmptyThenFaultIndicationShouldBeReceived)
{
    Server server(ioService_, sqlInterface_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.estabilishTcpConnection();

    size_t clientHash = 0x1234;
    sqlInterface_.pushQueryStatus(true);
    sqlInterface_.pushQueryResult(QueryResult());

    CheckAccountRequest request(clientHash, "Account", "Password");
    IO_CHECK(ioService_.send(connectionHash, request.getWriteStream()));

    const ReadStream &readStream = ioService_.receive(connectionHash);
    ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());

    FaultIndication indication(readStream);
    ASSERT_EQ(clientHash, indication.getClientHash());

    IO_CHECK(ioService_.disconnect(connectionHash));
}

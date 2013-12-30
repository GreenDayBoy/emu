#include <mt/env/dataserver/database/sqlInterfaceStub.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/check.hpp>
#include <dataserver/context.hpp>
#include <dataserver/protocol.hpp>
#include <protocol/readStream.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/faultIndication.hpp>

#include <core/network/tcp/networkUser.hpp>
#include <core/network/tcp/connection.hpp>

#include <gtest/gtest.h>

using eMU::dataserver::Context;
using eMU::dataserver::Protocol;
using eMU::dataserver::database::QueryResult;
using eMU::dataserver::database::Row;
using eMU::core::network::tcp::Connection;
using eMU::core::network::tcp::NetworkUser;
using eMU::core::network::Payload;
using eMU::mt::env::dataserver::database::SqlInterfaceStub;
using eMU::mt::env::asioStub::io_service;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::CheckAccountRequest;
using eMU::protocol::dataserver::CheckAccountResponse;
using eMU::protocol::dataserver::FaultIndication;
namespace MessageIds = eMU::protocol::dataserver::MessageIds;

class DataserverTest: public ::testing::Test
{
protected:
    DataserverTest():
        dataserverContext_(sqlInterface_, 5),
        dataserverProtocol_(dataserverContext_),
        connection_(new Connection(ioService_, dataserverProtocol_)),
        clientHash_(0x12345) {}

    void SetUp()
    {
        connection_->accept();
        ASSERT_EQ(1, dataserverContext_.getUsersFactory().getObjects().size());
    }

    void TearDown()
    {
        ASSERT_FALSE(connection_->getSocket().isUnread());

        connection_->getSocket().disconnect();
        ASSERT_EQ(0, dataserverContext_.getUsersFactory().getObjects().size());
    }

    void faultIndicationScenario()
    {
        CHECK(connection_->getSocket().send(CheckAccountRequest(clientHash_, "Account", "Password").getWriteStream().getPayload()));

        ASSERT_TRUE(connection_->getSocket().isUnread());
        const ReadStream &readStream = connection_->getSocket().receive();
        ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());

        FaultIndication indication(readStream);
        ASSERT_EQ(clientHash_, indication.getClientHash());
    }

    SqlInterfaceStub sqlInterface_;
    Context dataserverContext_;
    Protocol dataserverProtocol_;
    io_service ioService_;
    Connection::Pointer connection_;
    NetworkUser::Hash clientHash_;
};

TEST_F(DataserverTest, CheckAccountShouldBeSuccesful)
{
    QueryResult queryResult;
    Row &row = queryResult.createRow(Row::Fields());
    CheckAccountResult checkAccountResult = CheckAccountResult::AccountInUse;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(checkAccountResult)));

    sqlInterface_.pushQueryResult(queryResult);
    sqlInterface_.pushQueryStatus(true);

    CHECK(connection_->getSocket().send(CheckAccountRequest(clientHash_, "Account", "Password").getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &readStream = connection_->getSocket().receive();
    ASSERT_EQ(MessageIds::kCheckAccountResponse, readStream.getId());

    CheckAccountResponse response(readStream);
    ASSERT_EQ(clientHash_, response.getClientHash());
    ASSERT_EQ(checkAccountResult, response.getResult());
}

TEST_F(DataserverTest, WhenQueryExecutionFailedThenFaultIndicationShouldBeReceived)
{
    sqlInterface_.pushQueryStatus(false);

    faultIndicationScenario();
}

TEST_F(DataserverTest, WhenQueryResultIsEmptyThenFaultIndicationShouldBeReceived)
{
    sqlInterface_.pushQueryStatus(true);
    sqlInterface_.pushQueryResult(QueryResult());

    faultIndicationScenario();
}

TEST_F(DataserverTest, WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    sqlInterface_.setDied();

    faultIndicationScenario();
}

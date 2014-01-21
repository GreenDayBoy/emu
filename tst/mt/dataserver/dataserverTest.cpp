#include <mt/dataserver/database/sqlInterfaceStub.hpp>
#include <mt/asioStub/ioService.hpp>
#include <mt/check.hpp>
#include <dataserver/context.hpp>
#include <dataserver/protocol.hpp>
#include <streaming/readStream.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/dataserver/faultIndication.hpp>

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
using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::CheckAccountResult;
using eMU::streaming::dataserver::CheckAccountRequest;
using eMU::streaming::dataserver::CheckAccountResponse;
using eMU::streaming::dataserver::CharactersListRequest;
using eMU::streaming::dataserver::CharactersListResponse;
using eMU::streaming::dataserver::CharacterCreateRequest;
using eMU::streaming::dataserver::CharacterCreateResponse;
using eMU::streaming::dataserver::CharacterCreateResult;
using eMU::streaming::dataserver::FaultIndication;
namespace streamIds = eMU::streaming::dataserver::streamIds;
using eMU::streaming::common::CharacterViewInfo;

class DataserverTest: public ::testing::Test
{
protected:
    DataserverTest():
        dataserverContext_(sqlInterface_, 5),
        dataserverProtocol_(dataserverContext_),
        connection_(new Connection(ioService_, dataserverProtocol_)),
        userHash_(0x12345) {}

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

    void faultIndicationDueToQueryExecutionFailScenario(const Payload &payload)
    {
        sqlInterface_.pushQueryStatus(false);
        faultIndicationScenario(payload);
    }

    void faultIndicationDueToSqlConnectionDiedScenario(const Payload &payload)
    {
        sqlInterface_.setDied();
        faultIndicationScenario(payload);
    }

    void faultIndicationDueToEmptyQueryResultScenario(const Payload &payload)
    {
        sqlInterface_.pushQueryStatus(true);
        sqlInterface_.pushQueryResult(QueryResult());

        faultIndicationScenario(payload);
    }

    void faultIndicationScenario(const Payload &payload)
    {
        IO_CHECK(connection_->getSocket().send(payload));

        ASSERT_TRUE(connection_->getSocket().isUnread());
        const ReadStream &readStream = connection_->getSocket().receive();
        ASSERT_EQ(streamIds::kFaultIndication, readStream.getId());

        FaultIndication indication(readStream);
        ASSERT_EQ(userHash_, indication.getUserHash());
    }

    SqlInterfaceStub sqlInterface_;
    Context dataserverContext_;
    Protocol dataserverProtocol_;
    io_service ioService_;
    Connection::Pointer connection_;
    NetworkUser::Hash userHash_;
};

TEST_F(DataserverTest, CheckAccountShouldBeSuccesful)
{
    QueryResult queryResult;
    Row &row = queryResult.createRow(Row::Fields());
    CheckAccountResult checkAccountResult = CheckAccountResult::AccountInUse;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(checkAccountResult)));

    sqlInterface_.pushQueryResult(queryResult);
    sqlInterface_.pushQueryStatus(true);

    IO_CHECK(connection_->getSocket().send(CheckAccountRequest(userHash_, "Account", "Password").getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &readStream = connection_->getSocket().receive();
    ASSERT_EQ(streamIds::kCheckAccountResponse, readStream.getId());

    CheckAccountResponse response(readStream);
    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_EQ(checkAccountResult, response.getResult());
}

TEST_F(DataserverTest, CheckAccountRequest_QueryExecutionFailTriggersFaultIndication)
{
    faultIndicationDueToQueryExecutionFailScenario(CheckAccountRequest(userHash_, "Account", "Password").getWriteStream().getPayload());
}

TEST_F(DataserverTest, CheckAccountRequest_EmptyQueryResultTriggersFaultIndication)
{
    faultIndicationDueToEmptyQueryResultScenario(CheckAccountRequest(userHash_, "Account", "Password").getWriteStream().getPayload());
}

TEST_F(DataserverTest, CheckAccountRequest_WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    faultIndicationDueToSqlConnectionDiedScenario(CheckAccountRequest(userHash_, "Account", "Password").getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharactersListRequest_QueryExecutionFailTriggersFaultIndication)
{
    faultIndicationDueToQueryExecutionFailScenario(CharactersListRequest(userHash_, "acc").getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharactersListRequest_WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    faultIndicationDueToSqlConnectionDiedScenario(CharactersListRequest(userHash_, "acc").getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharactersList)
{
    sqlInterface_.pushQueryResult(QueryResult());
    sqlInterface_.pushQueryStatus(true);

    IO_CHECK(connection_->getSocket().send(CharactersListRequest(userHash_, "mu2emu").getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &readStream = connection_->getSocket().receive();
    ASSERT_EQ(streamIds::kCharactersListResponse, readStream.getId());

    CharactersListResponse response(readStream);
    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_TRUE(response.getCharacters().empty());
}

TEST_F(DataserverTest, CharacterCreateRequest_QueryExecutionFailTriggersFaultIndication)
{
    faultIndicationDueToQueryExecutionFailScenario(CharacterCreateRequest(userHash_, "acc",
                                                                          CharacterViewInfo("greg", 2, 4, 6,
                                                                                              8, 9, 11, 13, 15)).getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharacterCreateRequest_EmptyQueryResultTriggersFaultIndication)
{
    faultIndicationDueToEmptyQueryResultScenario(CharacterCreateRequest(userHash_, "acc",
                                                                        CharacterViewInfo("greg", 2, 4, 6,
                                                                                            8, 9, 11, 13, 15)).getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharacterCreateRequest_WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    faultIndicationDueToSqlConnectionDiedScenario(CharacterCreateRequest(userHash_, "acc",
                                                                         CharacterViewInfo("greg", 2, 4, 6,
                                                                                             8, 9, 11, 13, 15)).getWriteStream().getPayload());
}

TEST_F(DataserverTest, CharacterCreate)
{
    QueryResult queryResult;
    Row &row = queryResult.createRow(Row::Fields());
    CharacterCreateResult result = CharacterCreateResult::Succeed;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(result)));

    sqlInterface_.pushQueryResult(queryResult);
    sqlInterface_.pushQueryStatus(true);

    IO_CHECK(connection_->getSocket().send(CharacterCreateRequest(userHash_, "mu2account",
                                                                  CharacterViewInfo("mu2c", 2, 4, 6,
                                                                                      8, 9, 11, 13, 15)).getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &readStream = connection_->getSocket().receive();
    ASSERT_EQ(streamIds::kCharacterCreateResponse, readStream.getId());

    CharacterCreateResponse response(readStream);
    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_EQ(result, response.getResult());
}

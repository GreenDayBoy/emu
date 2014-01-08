#include <dataserver/transactions/charactersListRequest.hpp>
#include <dataserver/user.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/dataserver/faultIndication.hpp>

#include <ut/dataserver/database/sqlInterfaceMock.hpp>
#include <ut/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

using eMU::ut::env::core::network::tcp::ConnectionMock;
using eMU::ut::env::dataserver::database::SqlInterfaceMock;

using eMU::dataserver::User;
using eMU::dataserver::database::QueryResult;
using eMU::dataserver::database::Row;
namespace transactions = eMU::dataserver::transactions;

using eMU::streaming::ReadStream;
namespace streamIds = eMU::streaming::dataserver::streamIds;

using eMU::streaming::dataserver::CharactersListRequest;
using eMU::streaming::dataserver::CharactersListResponse;
using eMU::streaming::dataserver::FaultIndication;

using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class DataserverCharactersListRequestTransactionTest: public ::testing::Test
{
protected:
    DataserverCharactersListRequestTransactionTest():
        userHash_(0x53212),
        connection_(new ConnectionMock()),
        user_(connection_),
        request_(ReadStream(CharactersListRequest(userHash_, "account").getWriteStream().getPayload())),
        transaction_(user_, sqlInterface_, request_) {}

    SqlInterfaceMock sqlInterface_;
    QueryResult queryResult_;
    Payload payload_;

    NetworkUser::Hash userHash_;
    ConnectionMock::Pointer connection_;
    User user_;
    CharactersListRequest request_;
    transactions::CharactersListRequest transaction_;
};

TEST_F(DataserverCharactersListRequestTransactionTest, handle)
{
    Row::Fields fields = {{"hairColor", 0}, {"hairType", 1}, {"level", 2},
                          {"name", 3}, {"race", 4}, {"tutorialState", 5}};

    Row *row = &queryResult_.createRow(fields);
    row->insert("12"); row->insert("23"); row->insert("45"); row->insert("andrew"); row->insert("44"); row->insert("0");

    row = &queryResult_.createRow(fields);
    row->insert("55"); row->insert("64"); row->insert("178"); row->insert("greg"); row->insert("81"); row->insert("1");

    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kCharactersListResponse, readStream.getId());
    CharactersListResponse response(readStream);

    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_EQ(2, response.getCharacters().size());

    EXPECT_EQ(12, response.getCharacters()[0].hairColor_);
    EXPECT_EQ(23, response.getCharacters()[0].hairType_);
    EXPECT_EQ(45, response.getCharacters()[0].level_);
    EXPECT_EQ("andrew", response.getCharacters()[0].name_);
    EXPECT_EQ(44, response.getCharacters()[0].race_);
    EXPECT_EQ(0, response.getCharacters()[0].tutorialState_);

    EXPECT_EQ(55, response.getCharacters()[1].hairColor_);
    EXPECT_EQ(64, response.getCharacters()[1].hairType_);
    EXPECT_EQ(178, response.getCharacters()[1].level_);
    EXPECT_EQ("greg", response.getCharacters()[1].name_);
    EXPECT_EQ(81, response.getCharacters()[1].race_);
    EXPECT_EQ(1, response.getCharacters()[1].tutorialState_);
}

TEST_F(DataserverCharactersListRequestTransactionTest, WhenExecutionOfQueryIsFailedThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(false));

    std::string errorMessage = "database error";
    EXPECT_CALL(sqlInterface_, getErrorMessage()).WillOnce(Return(errorMessage));

    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(userHash_, indication.getUserHash());
    ASSERT_EQ(errorMessage, indication.getMessage());
}

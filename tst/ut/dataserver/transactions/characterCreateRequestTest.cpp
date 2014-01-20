#include <dataserver/transactions/characterCreateRequest.hpp>
#include <dataserver/user.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/dataserver/faultIndication.hpp>

#include <ut/dataserver/database/sqlInterfaceMock.hpp>
#include <ut/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

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

using eMU::streaming::common::CharacterCreateInfo;
using eMU::streaming::dataserver::CharacterCreateResult;
using eMU::streaming::dataserver::CharacterCreateRequest;
using eMU::streaming::dataserver::CharacterCreateResponse;
using eMU::streaming::dataserver::FaultIndication;

using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class DataserverCharacterCreateRequestTransactionTest: public ::testing::Test
{
protected:
    DataserverCharacterCreateRequestTransactionTest():
        userHash_(0x12345),
        connection_(new ConnectionMock()),
        user_(connection_),
        request_(ReadStream(CharacterCreateRequest(userHash_, "simpleAccount",
                                                   CharacterCreateInfo("andrew", 8, 7, 6, 5, 4, 3, 2, 1)).getWriteStream().getPayload())),
        transaction_(user_, sqlInterface_, request_) {}

    SqlInterfaceMock sqlInterface_;
    QueryResult queryResult_;
    Payload payload_;

    NetworkUser::Hash userHash_;
    ConnectionMock::Pointer connection_;
    User user_;
    CharacterCreateRequest request_;
    transactions::CharacterCreateRequest transaction_;
};

TEST_F(DataserverCharacterCreateRequestTransactionTest, handle)
{
    Row &row = queryResult_.createRow(Row::Fields());
    CharacterCreateResult result = CharacterCreateResult::CharactersCountExceeded;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(result)));

    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kCharacterCreateResponse, readStream.getId());
    CharacterCreateResponse response(readStream);

    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_EQ(result, response.getResult());
}

TEST_F(DataserverCharacterCreateRequestTransactionTest, WhenExecutionOfQueryIsFailedThenFaultIndicationShouldBeSent)
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

TEST_F(DataserverCharacterCreateRequestTransactionTest, WhenQueryResultIsEmptyThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(userHash_, indication.getUserHash());
}

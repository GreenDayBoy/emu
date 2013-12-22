#include <dataserver/transactions/checkAccountRequest.hpp>
#include <dataserver/user.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <protocol/readStream.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/faultIndication.hpp>

#include <ut/env/dataserver/database/sqlInterfaceMock.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

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

using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
namespace MessageIds = eMU::protocol::dataserver::MessageIds;

using eMU::protocol::dataserver::CheckAccountRequest;
using eMU::protocol::dataserver::CheckAccountResponse;
using eMU::protocol::dataserver::FaultIndication;

using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class CheckAccountRequestTransactionTest: public ::testing::Test
{
protected:
    CheckAccountRequestTransactionTest():
        clientHash_(0x12345),
        user_(connection_),
        request_(ReadStream(CheckAccountRequest(clientHash_,
                                                "testAccount",
                                                "testPassword").getWriteStream().getPayload())) {}

    SqlInterfaceMock sqlInterface_;
    QueryResult queryResult_;
    Payload payload_;

    NetworkUser::Hash clientHash_;
    ConnectionMock connection_;
    User user_;
    CheckAccountRequest request_;
};

TEST_F(CheckAccountRequestTransactionTest, handle)
{
    Row &row = queryResult_.createRow(Row::Fields());
    CheckAccountResult result = CheckAccountResult::AccountInUse;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(result)));

    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    eMU::dataserver::transactions::CheckAccountRequest(user_, sqlInterface_, request_).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(MessageIds::kCheckAccountResponse, readStream.getId());
    CheckAccountResponse response(readStream);

    ASSERT_EQ(clientHash_, response.getClientHash());
    ASSERT_EQ(result, response.getResult());
}

TEST_F(CheckAccountRequestTransactionTest, WhenExecutionOfQueryIsFailedThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(false));

    std::string errorMessage = "database error";
    EXPECT_CALL(sqlInterface_, getErrorMessage()).WillOnce(Return(errorMessage));

    EXPECT_CALL(connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    eMU::dataserver::transactions::CheckAccountRequest(user_, sqlInterface_, request_).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(clientHash_, indication.getClientHash());
    ASSERT_EQ(errorMessage, indication.getMessage());
}

TEST_F(CheckAccountRequestTransactionTest, WhenQueryResultIsEmptyThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    eMU::dataserver::transactions::CheckAccountRequest(user_, sqlInterface_, request_).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(clientHash_, indication.getClientHash());
}

TEST_F(CheckAccountRequestTransactionTest, WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(false));
    EXPECT_CALL(connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    eMU::dataserver::transactions::CheckAccountRequest(user_, sqlInterface_, request_).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(MessageIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(clientHash_, indication.getClientHash());
}

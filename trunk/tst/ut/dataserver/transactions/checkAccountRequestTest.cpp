#include <dataserver/transactions/checkAccountRequest.hpp>
#include <dataserver/user.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
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
using eMU::streaming::dataserver::CheckAccountResult;
namespace streamIds = eMU::streaming::dataserver::streamIds;

using eMU::streaming::dataserver::CheckAccountRequest;
using eMU::streaming::dataserver::CheckAccountResponse;
using eMU::streaming::dataserver::FaultIndication;

using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class CheckAccountRequestTransactionTest: public ::testing::Test
{
protected:
    CheckAccountRequestTransactionTest():
        userHash_(0x12345),
        connection_(new ConnectionMock()),
        user_(connection_),
        request_(ReadStream(CheckAccountRequest(userHash_,
                                                "testAccount",
                                                "testPassword").getWriteStream().getPayload())),
        transaction_(user_, sqlInterface_, request_) {}

    SqlInterfaceMock sqlInterface_;
    QueryResult queryResult_;
    Payload payload_;

    NetworkUser::Hash userHash_;
    ConnectionMock::Pointer connection_;
    User user_;
    CheckAccountRequest request_;
    transactions::CheckAccountRequest transaction_;
};

TEST_F(CheckAccountRequestTransactionTest, handle)
{
    Row &row = queryResult_.createRow(Row::Fields());
    CheckAccountResult result = CheckAccountResult::AccountInUse;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(result)));

    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kCheckAccountResponse, readStream.getId());
    CheckAccountResponse response(readStream);

    ASSERT_EQ(userHash_, response.getUserHash());
    ASSERT_EQ(result, response.getResult());
}

TEST_F(CheckAccountRequestTransactionTest, WhenExecutionOfQueryIsFailedThenFaultIndicationShouldBeSent)
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

TEST_F(CheckAccountRequestTransactionTest, WhenQueryResultIsEmptyThenFaultIndicationShouldBeSent)
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

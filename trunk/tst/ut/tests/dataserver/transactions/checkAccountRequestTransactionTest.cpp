#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

#include <protocol/readStream.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/CheckAccountResult.hpp>
#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <ut/env/dataserver/database/sqlInterfaceMock.hpp>

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

using eMU::ut::env::core::network::tcp::ConnectionsManagerMock;
using eMU::ut::env::dataserver::database::SqlInterfaceMock;

using eMU::dataserver::database::QueryResult;
using eMU::dataserver::database::Row;
using eMU::dataserver::transactions::CheckAccountRequestTransaction;

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::decoders::CheckAccountRequest;
using eMU::protocol::ReadStream;

using eMU::core::network::Payload;

class CheckAccountRequestTransactionTest: public ::testing::Test
{
public:
    CheckAccountRequestTransactionTest():
        clientHash_(0x12345),
        hash_(0x54321),
        request_(clientHash_, "testAccount", "testPassword") {}

    ConnectionsManagerMock connectionsManager_;
    SqlInterfaceMock sqlInterface_;
    QueryResult queryResult_;
    Payload payload_;

    size_t clientHash_;
    size_t hash_;
    CheckAccountRequest request_;
};

TEST_F(CheckAccountRequestTransactionTest, handle)
{
    Row &row = queryResult_.createRow(Row::Fields());
    CheckAccountResult result = CheckAccountResult::AcoountInUse;
    row.insert(boost::lexical_cast<Row::Value>(static_cast<uint32_t>(result)));

    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));
    EXPECT_CALL(sqlInterface_, executeQuery(_));
    EXPECT_CALL(connectionsManager_, send(hash_, _)).WillOnce(SaveArg<1>(&payload_));

    CheckAccountRequestTransaction(hash_, sqlInterface_, connectionsManager_, request_).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(MessageIds::kCheckAccountResponse, readStream.getId());
    ASSERT_EQ(clientHash_, readStream.readNext<size_t>());
    ASSERT_EQ(result, readStream.readNext<CheckAccountResult>());
}

TEST_F(CheckAccountRequestTransactionTest, isValidShouldAlawayReturnTrue)
{
    CheckAccountRequestTransaction transaction(hash_, sqlInterface_, connectionsManager_, request_);

    ASSERT_TRUE(transaction.isValid());
}

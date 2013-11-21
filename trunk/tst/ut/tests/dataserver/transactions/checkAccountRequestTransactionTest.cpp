#include <dataserver/transactions/checkAccountRequestTransaction.hpp>
#include <protocol/readStream.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/CheckAccountResult.hpp>
#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <ut/env/dataserver/database/sqlInterfaceMock.hpp>

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

namespace dataserverEnv = eMU::ut::env::dataserver;
namespace networkEnv = eMU::ut::env::core::network;

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

class CheckAccountRequestTransactionTest: public ::testing::Test
{
public:
    CheckAccountRequestTransactionTest():
        clientHash_(0x12345),
        hash_(0x54321),
        request_(clientHash_, "testAccount", "testPassword") {}

    networkEnv::tcp::ConnectionsManagerMock connectionsManager_;
    dataserverEnv::database::SqlInterfaceMock sqlInterface_;
    eMU::dataserver::database::QueryResult queryResult_;
    eMU::core::network::Payload payload_;

    size_t clientHash_;
    size_t hash_;
    eMU::protocol::dataserver::decoders::CheckAccountRequest request_;
};

TEST_F(CheckAccountRequestTransactionTest, handle)
{
    uint8_t result = static_cast<uint8_t>(eMU::protocol::dataserver::CheckAccountResult::Succeed);
    queryResult_.getRows().push_back({boost::lexical_cast<std::string>(result)});
    EXPECT_CALL(sqlInterface_, fetchQueryResult()).WillOnce(Return((queryResult_)));

    EXPECT_CALL(sqlInterface_, executeQuery(_));
    EXPECT_CALL(connectionsManager_, send(hash_, _)).WillOnce(SaveArg<1>(&payload_));


    eMU::dataserver::transactions::CheckAccountRequestTransaction(hash_, sqlInterface_, connectionsManager_, request_).handle();

    eMU::protocol::ReadStream readStream(payload_);
    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountResponse, readStream.getId());
    ASSERT_EQ(clientHash_, readStream.readNext<size_t>());
    ASSERT_EQ(result, readStream.readNext<uint8_t>());
}

TEST_F(CheckAccountRequestTransactionTest, isValidShouldAlawayReturnTrue)
{
    eMU::dataserver::transactions::CheckAccountRequestTransaction transaction(hash_, sqlInterface_, connectionsManager_, request_);

    ASSERT_TRUE(transaction.isValid());
}

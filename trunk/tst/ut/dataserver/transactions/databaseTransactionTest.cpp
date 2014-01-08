#include <dataserver/transactions/databaseTransaction.hpp>
#include <dataserver/user.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/streamIds.hpp>
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
using eMU::dataserver::transactions::DatabaseTransaction;

using eMU::streaming::ReadStream;
namespace streamIds = eMU::streaming::dataserver::streamIds;
using eMU::streaming::dataserver::FaultIndication;

using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class DatabaseTransactionTest: public ::testing::Test
{
protected:
    DatabaseTransactionTest():
        userHash_(0x12345),
        connection_(new ConnectionMock()),
        user_(connection_),
        transaction_(user_, sqlInterface_, userHash_) {}

    NetworkUser::Hash userHash_;
    ConnectionMock::Pointer connection_;
    User user_;
    SqlInterfaceMock sqlInterface_;
    DatabaseTransaction transaction_;
};

TEST_F(DatabaseTransactionTest, WhenConnectionToDatabaseIsDiedThenFaultIndicationShouldBeSent)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(false));

    Payload payload;
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload));

    transaction_.handle();

    ReadStream readStream(payload);
    ASSERT_EQ(streamIds::kFaultIndication, readStream.getId());
    FaultIndication indication(readStream);

    ASSERT_EQ(userHash_, indication.getUserHash());
}

TEST_F(DatabaseTransactionTest, WhenConnectionToDatabaseIsAliveThenNothingHappens)
{
    EXPECT_CALL(sqlInterface_, isAlive()).WillOnce(Return(true));

    transaction_.handle();
}

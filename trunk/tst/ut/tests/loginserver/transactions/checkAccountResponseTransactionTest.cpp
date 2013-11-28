#include <loginserver/transactions/checkAccountResponseTransaction.hpp>
#include <loginserver/user.hpp>
#include <core/common/usersFactory.hpp>
#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <protocol/dataserver/decoders/checkAccountResponse.hpp>
#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/decoders/loginResponse.hpp>

using ::testing::_;
using ::testing::SaveArg;

using eMU::loginserver::User;
using eMU::loginserver::transactions::CheckAccountResponseTransaction;
using eMU::core::common::UsersFactory;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::tcp::ConnectionsManagerMock;

namespace encoders = eMU::protocol::dataserver::encoders;
namespace decoders = eMU::protocol::dataserver::decoders;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::ReadStream;
namespace MessageIds = eMU::protocol::loginserver::MessageIds;
using eMU::protocol::loginserver::decoders::LoginResponse;
using eMU::protocol::loginserver::LoginResult;

class CheckAccountResponseTransactionTest: public ::testing::Test
{
protected:
    CheckAccountResponseTransactionTest():
        usersFactory_(1) {}

    UsersFactory<User> usersFactory_;
    ConnectionsManagerMock connectionsManager_;
    Payload payload_;

    void handle(CheckAccountResult checkAccountResult, LoginResult expectedLoginResult)
    {
        size_t hash = usersFactory_.create();
        decoders::CheckAccountResponse checkAccountResponse(encoders::CheckAccountResponse(hash, checkAccountResult).getWriteStream().getPayload());

        EXPECT_CALL(connectionsManager_, send(hash, _)).WillOnce(SaveArg<1>(&payload_));
        CheckAccountResponseTransaction(connectionsManager_, usersFactory_, checkAccountResponse).handle();

        ReadStream readStream(payload_);
        ASSERT_EQ(MessageIds::kLoginResponse, readStream.getId());

        LoginResponse loginResponse(readStream);
        ASSERT_EQ(expectedLoginResult, loginResponse.getResult());
    }
};

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnSucceedThenLoginResponseShouldBeSucceed)
{
    handle(CheckAccountResult::Succeed, LoginResult::Succeed);
}

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnFailedThenLoginResponseShouldBeFailed)
{
    handle(CheckAccountResult::Failed, LoginResult::Failed);
}

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnAccountInUseThenLoginResponseShouldBeFailed)
{
    handle(CheckAccountResult::AcoountInUse, LoginResult::Failed);
}

TEST_F(CheckAccountResponseTransactionTest, WhenHashGivenFromDataServerIsInvalidThenNothingShouldHappen)
{
    decoders::CheckAccountResponse checkAccountResponse(encoders::CheckAccountResponse(0x12345, CheckAccountResult::Succeed).getWriteStream().getPayload());

    CheckAccountResponseTransaction(connectionsManager_, usersFactory_, checkAccountResponse).handle();
}

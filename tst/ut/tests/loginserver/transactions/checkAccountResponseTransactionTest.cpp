#include <loginserver/transactions/checkAccountResponseTransaction.hpp>
#include <loginserver/user.hpp>
#include <core/common/usersFactory.hpp>
#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/loginResponse.hpp>

using ::testing::_;
using ::testing::SaveArg;

using eMU::loginserver::User;
using eMU::loginserver::transactions::CheckAccountResponseTransaction;
using eMU::core::common::UsersFactory;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::tcp::ConnectionsManagerMock;

using eMU::protocol::dataserver::CheckAccountResponse;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::ReadStream;
namespace MessageIds = eMU::protocol::loginserver::MessageIds;
using eMU::protocol::loginserver::LoginResponse;
using eMU::protocol::loginserver::LoginResult;

class CheckAccountResponseTransactionTest: public ::testing::Test
{
protected:
    CheckAccountResponseTransactionTest():
        usersFactory_(1),
        checkAccountResult_(CheckAccountResult::Succeed),
        expectedLoginResult_(LoginResult::Succeed) {}

    void scenario()
    {
        size_t hash = usersFactory_.create();
        CheckAccountResponse checkAccountResponse(CheckAccountResponse(hash, checkAccountResult_).getWriteStream().getPayload());

        EXPECT_CALL(connectionsManager_, send(hash, _)).WillOnce(SaveArg<1>(&payload_));
        CheckAccountResponseTransaction(connectionsManager_, usersFactory_, checkAccountResponse).handle();

        ReadStream readStream(payload_);
        ASSERT_EQ(MessageIds::kLoginResponse, readStream.getId());

        LoginResponse loginResponse(readStream);
        ASSERT_EQ(expectedLoginResult_, loginResponse.getResult());
    }

    UsersFactory<User> usersFactory_;
    ConnectionsManagerMock connectionsManager_;
    Payload payload_;
    CheckAccountResult checkAccountResult_;
    LoginResult expectedLoginResult_;
};

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnSucceedThenLoginResponseShouldBeSucceed)
{
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnFailedThenLoginResponseShouldBeFailed)
{
    checkAccountResult_ = CheckAccountResult::Failed;
    expectedLoginResult_ = LoginResult::Failed;
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnAccountInUseThenLoginResponseShouldBeFailed)
{
    checkAccountResult_ = CheckAccountResult::AcoountInUse;
    expectedLoginResult_ = LoginResult::Failed;
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, WhenHashGivenFromDataServerIsInvalidThenNothingShouldHappen)
{
    CheckAccountResponse checkAccountResponse(CheckAccountResponse(0x12345, CheckAccountResult::Succeed).getWriteStream().getPayload());

    CheckAccountResponseTransaction(connectionsManager_, usersFactory_, checkAccountResponse).handle();
}

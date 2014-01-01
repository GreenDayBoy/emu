#include <loginserver/transactions/checkAccountResponse.hpp>
#include <loginserver/user.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/loginserver/messageIds.hpp>
#include <streaming/loginserver/loginResponse.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::SaveArg;

using eMU::loginserver::User;
using eMU::core::common::Factory;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::tcp::ConnectionMock;

using eMU::streaming::dataserver::CheckAccountResponse;
using eMU::streaming::dataserver::CheckAccountResult;
using eMU::streaming::ReadStream;
namespace MessageIds = eMU::streaming::loginserver::MessageIds;
using eMU::streaming::loginserver::LoginResponse;
using eMU::streaming::loginserver::LoginResult;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class CheckAccountResponseTransactionTest: public ::testing::Test
{
protected:
    CheckAccountResponseTransactionTest():
        checkAccountResult_(CheckAccountResult::Succeed),
        expectedLoginResult_(LoginResult::Succeed),
        connection_(new ConnectionMock()) {}

    void scenario()
    {
        User &user = usersFactory_.create(connection_);
        CheckAccountResponse checkAccountResponse(CheckAccountResponse(user.getHash(), checkAccountResult_).getWriteStream().getPayload());

        EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));
        eMU::loginserver::transactions::CheckAccountResponse(usersFactory_, checkAccountResponse).handle();

        ReadStream readStream(payload_);
        ASSERT_EQ(MessageIds::kLoginResponse, readStream.getId());

        LoginResponse loginResponse(readStream);
        ASSERT_EQ(expectedLoginResult_, loginResponse.getResult());
    }

    Factory<User> usersFactory_;
    Payload payload_;
    CheckAccountResult checkAccountResult_;
    LoginResult expectedLoginResult_;
    ConnectionMock::Pointer connection_;
};

TEST_F(CheckAccountResponseTransactionTest, WhenDataServerReturnSucceedThenLoginResponseShouldBeSucceed)
{
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, CheckInvalidAccountId)
{
    checkAccountResult_ = CheckAccountResult::InvalidAccountId;
    expectedLoginResult_ = LoginResult::InvalidAccountId;
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, CheckAccountInUse)
{
    checkAccountResult_ = CheckAccountResult::AccountInUse;
    expectedLoginResult_ = LoginResult::AccountInUse;
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, CheckInvalidPassword)
{
    checkAccountResult_ = CheckAccountResult::InvalidPassword;
    expectedLoginResult_ = LoginResult::InvalidPassword;
    scenario();
}

TEST_F(CheckAccountResponseTransactionTest, WhenHashGivenFromDataServerIsInvalidThenNothingShouldHappen)
{
    usersFactory_.create(connection_);

    CheckAccountResponse checkAccountResponse(CheckAccountResponse(User::Hash(0x12345), CheckAccountResult::Succeed).getWriteStream().getPayload());

    eMU::loginserver::transactions::CheckAccountResponse(usersFactory_, checkAccountResponse).handle();
}

#include <loginserver/transactions/registerUserResponse.hpp>
#include <loginserver/user.hpp>
#include <core/network/payload.hpp>
#include <core/common/factory.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>

#include <ut/core/network/tcp/connectionMock.hpp>
#include <ut/loginserver/gameserversListMock.hpp>

#include <gtest/gtest.h>

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;
using ::testing::SaveArg;

using eMU::streaming::ReadStream;
using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;
using eMU::core::common::Factory;
using eMU::loginserver::User;
using eMU::streaming::loginserver::GameserverInfo;
using eMU::streaming::loginserver::GameserverDetailsResponse;
using eMU::streaming::gameserver::RegisterUserResponse;
using eMU::streaming::gameserver::UserRegistrationResult;
namespace streamIds = eMU::streaming::loginserver::streamIds;
using eMU::ut::env::loginserver::GameserversListMock;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class RegisterUserResponseTransactionTest: public ::testing::Test
{
protected:
    RegisterUserResponseTransactionTest():
        connection_(new eMU::ut::env::core::network::tcp::ConnectionMock()),
        gameserverCode_(234) {}

    ConnectionMock::Pointer connection_;
    GameserversListMock gameserversList_;
    Factory<User> usersFactory_;
    uint16_t gameserverCode_;
    Payload payload_;
};

TEST_F(RegisterUserResponseTransactionTest, handle)
{
    User &user = usersFactory_.create(connection_);
    RegisterUserResponse response(ReadStream(RegisterUserResponse(gameserverCode_,
                                                                 user.getHash(),
                                                                 UserRegistrationResult::Succeed).getWriteStream().getPayload()));

    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(true));

    GameserverInfo gameserverInfo = {gameserverCode_, "eMU_Test12", "192.168.0.1", 55905};
    EXPECT_CALL(gameserversList_, getGameserverInfo(gameserverCode_)).WillOnce(ReturnRef(gameserverInfo));

    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));
    eMU::loginserver::transactions::RegisterUserResponse(usersFactory_, gameserversList_, response).handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kGameserverDetailsResponse, readStream.getId());

    GameserverDetailsResponse gameserverDetailsResponse(readStream);
    ASSERT_EQ(gameserverInfo.address_, gameserverDetailsResponse.getIpAddress());
    ASSERT_EQ(gameserverInfo.port_, gameserverDetailsResponse.getPort());
}

TEST_F(RegisterUserResponseTransactionTest, WhenUserRegistrationFailedThenConnectionShouldBeDisconnect)
{
    User &user = usersFactory_.create(connection_);
    RegisterUserResponse response(ReadStream(RegisterUserResponse(gameserverCode_,
                                                                 user.getHash(),
                                                                 UserRegistrationResult::Failed).getWriteStream().getPayload()));

    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, disconnect());

    eMU::loginserver::transactions::RegisterUserResponse(usersFactory_, gameserversList_, response).handle();
}

TEST_F(RegisterUserResponseTransactionTest, WhenGameserverDoesNotExistThenNothingShouldHappen)
{
    User &user = usersFactory_.create(connection_);
    RegisterUserResponse response(ReadStream(RegisterUserResponse(gameserverCode_,
                                                                 user.getHash(),
                                                                 UserRegistrationResult::Succeed).getWriteStream().getPayload()));

    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(false));
    eMU::loginserver::transactions::RegisterUserResponse(usersFactory_, gameserversList_, response).handle();
}

TEST_F(RegisterUserResponseTransactionTest, WhenUserDoesNotExistThenNothingShouldHappen)
{
    RegisterUserResponse response(ReadStream(RegisterUserResponse(gameserverCode_,
                                                                  NetworkUser::Hash(0x1234),
                                                                 UserRegistrationResult::Succeed).getWriteStream().getPayload()));

    eMU::loginserver::transactions::RegisterUserResponse(usersFactory_, gameserversList_, response).handle();
}

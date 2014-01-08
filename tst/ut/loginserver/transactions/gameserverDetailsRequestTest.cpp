#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <loginserver/user.hpp>
#include <core/network/payload.hpp>
#include <streaming/loginserver/gameserverInfo.hpp>
#include <streaming/loginserver/gameserverDetailsRequest.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>

#include <streaming/gameserver/streamIds.hpp>

#include <ut/core/network/tcp/connectionMock.hpp>
#include <ut/core/network/udp/connectionMock.hpp>
#include <ut/loginserver/gameserversListMock.hpp>

#include <gtest/gtest.h>

using eMU::streaming::ReadStream;
using eMU::core::network::Payload;
using eMU::loginserver::User;
using eMU::ut::env::loginserver::GameserversListMock;
namespace streamIds = eMU::streaming::gameserver::streamIds;
using eMU::streaming::loginserver::GameserverDetailsRequest;
using eMU::streaming::loginserver::GameserverInfo;
using eMU::streaming::gameserver::RegisterUserRequest;

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;
using ::testing::SaveArg;

class GameserverDetailsRequestTransactionTest: public ::testing::Test
{
protected:
    GameserverDetailsRequestTransactionTest():
        connection_(new eMU::ut::env::core::network::tcp::ConnectionMock()),
        udpConnection_(new eMU::ut::env::core::network::udp::ConnectionMock()),
        user_(connection_),
        gameserverCode_(123),
        request_(ReadStream(GameserverDetailsRequest(gameserverCode_).getWriteStream().getPayload())),
        transaction_(user_, gameserversList_, udpConnection_, request_) {}

    eMU::ut::env::core::network::tcp::ConnectionMock::Pointer connection_;
    eMU::ut::env::core::network::udp::ConnectionMock::Pointer udpConnection_;
    User user_;
    GameserversListMock gameserversList_;
    uint16_t gameserverCode_;
    GameserverDetailsRequest request_;
    eMU::loginserver::transactions::GameserverDetailsRequest transaction_;
    Payload payload_;
};

TEST_F(GameserverDetailsRequestTransactionTest, handle)
{
    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(true));

    GameserverInfo gameserverInfo = {gameserverCode_, "eMU_Test", "127.0.0.1", 55901};
    EXPECT_CALL(gameserversList_, getGameserverInfo(gameserverCode_)).WillOnce(ReturnRef(gameserverInfo));
    EXPECT_CALL(*udpConnection_, sendTo(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(gameserverInfo.address_),
                                                                       gameserverInfo.port_),
                                        _)).WillOnce(SaveArg<1>(&payload_));

    user_.setAccountId("testAccount");
    transaction_.handle();

    ReadStream readStream(payload_);
    ASSERT_EQ(streamIds::kRegisterUserRequest, readStream.getId());

    RegisterUserRequest registerUserRequest(readStream);
    ASSERT_EQ(user_.getAccountId(), registerUserRequest.getUserRegistrationInfo().accountId_);
    ASSERT_EQ(user_.getHash(), registerUserRequest.getUserRegistrationInfo().userHash_);
}

TEST_F(GameserverDetailsRequestTransactionTest, WhenGameserverCodeIsInvalidThenConnectionShouldBeDisconnect)
{
    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(false));
    EXPECT_CALL(*connection_, disconnect());

    transaction_.handle();
}

TEST_F(GameserverDetailsRequestTransactionTest, WhenUdpConnectionIsNullptrThenConnectionShouldBeDisconnect)
{
    EXPECT_CALL(gameserversList_, hasGameserver(gameserverCode_)).WillOnce(Return(true));
    EXPECT_CALL(*connection_, disconnect());

    eMU::loginserver::transactions::GameserverDetailsRequest(user_, gameserversList_, nullptr, request_).handle();
}

#include <gameserver/transactions/registerUserRequest.hpp>
#include <streaming/gameserver/userRegistrationInfo.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <ut/env/core/network/udp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::SaveArg;

using eMU::streaming::ReadStream;
using eMU::streaming::gameserver::UserRegistrationInfoContainer;
using eMU::streaming::gameserver::RegisterUserRequest;
using eMU::streaming::gameserver::RegisterUserResponse;
using eMU::streaming::gameserver::UserRegistrationResult;
namespace streamIds = eMU::streaming::gameserver::streamIds;
using eMU::core::network::Payload;
using eMU::core::network::tcp::NetworkUser;

class RegisterUserRequestTransactionTest: public ::testing::Test
{
protected:
    RegisterUserRequestTransactionTest():
        senderEndpoint_(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("10.0.0.3"), 55431)),
        udpConnection_(new eMU::ut::env::core::network::udp::ConnectionMock()),
        gameserverCode_(642),
        userHash_(0x45621),
        accountId_("userAccount"),
        request_(ReadStream(RegisterUserRequest({userHash_, accountId_}).getWriteStream().getPayload())) {}

    void scenario(UserRegistrationResult result)
    {
        EXPECT_CALL(*udpConnection_, sendTo(senderEndpoint_, _)).WillOnce(SaveArg<1>(&payload_));

        if(result == UserRegistrationResult::Failed)
        {
            userRegistrationInfos_.push_back({userHash_, accountId_});
        }

        eMU::gameserver::transactions::RegisterUserRequest(senderEndpoint_,
                                                           udpConnection_,
                                                           userRegistrationInfos_,
                                                           gameserverCode_,
                                                           request_).handle();

        ReadStream readStream(payload_);
        ASSERT_EQ(streamIds::kRegisterUserResponse, readStream.getId());

        RegisterUserResponse response(readStream);
        ASSERT_EQ(gameserverCode_, response.getGameserverCode());
        ASSERT_EQ(userHash_, response.getUserHash());
        ASSERT_EQ(result, response.getResult());

        if(result == UserRegistrationResult::Succeed)
        {
            ASSERT_EQ(1, userRegistrationInfos_.size());
            ASSERT_EQ(userHash_, userRegistrationInfos_.back().userHash_);
            ASSERT_EQ(accountId_, userRegistrationInfos_.back().accountId_);
        }
    }

    boost::asio::ip::udp::endpoint senderEndpoint_;
    eMU::ut::env::core::network::udp::ConnectionMock::Pointer udpConnection_;
    UserRegistrationInfoContainer userRegistrationInfos_;
    uint16_t gameserverCode_;
    NetworkUser::Hash userHash_;
    std::string accountId_;
    RegisterUserRequest request_;
    Payload payload_;

};

TEST_F(RegisterUserRequestTransactionTest, WhenUserRegistrationInfoDoesNotExistThenRegistrationShouldBeSucced)
{
    scenario(UserRegistrationResult::Succeed);
}

TEST_F(RegisterUserRequestTransactionTest, WhenUserRegistrationInfoExistsThenRegistrationShouldBeFailed)
{
    scenario(UserRegistrationResult::Failed);
}

TEST_F(RegisterUserRequestTransactionTest, WhenUdpConnectionIsNullptrThenNothingShouldHappen)
{
    eMU::gameserver::transactions::RegisterUserRequest(senderEndpoint_,
                                                       nullptr,
                                                       userRegistrationInfos_,
                                                       gameserverCode_,
                                                       request_).handle();
}

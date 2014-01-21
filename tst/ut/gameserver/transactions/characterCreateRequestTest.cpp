#include <gameserver/transactions/characterCreateRequest.hpp>
#include <gameserver/user.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/gameserver/characterCreateRequest.hpp>

#include <ut/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

using eMU::ut::env::core::network::tcp::ConnectionMock;
using eMU::core::network::Payload;
using eMU::streaming::ReadStream;
using eMU::gameserver::User;
using eMU::streaming::dataserver::CharacterCreateRequest;
using eMU::streaming::common::CharacterViewInfo;
namespace streamIds = eMU::streaming::dataserver::streamIds;

class GameserverCharacterCreateRequestTransactionTest: public ::testing::Test
{
protected:
    GameserverCharacterCreateRequestTransactionTest():
        accountId_("testtesttest"),
        request_(ReadStream(eMU::streaming::gameserver::CharacterCreateRequest(CharacterViewInfo("andrew", 1, 2, 3, 4, 5, 6, 7, 8)).getWriteStream().getPayload())),
        dataserverConnection_(new ConnectionMock()),
        connection_(new ConnectionMock()),
        user_(connection_) {}

    std::string accountId_;
    eMU::streaming::gameserver::CharacterCreateRequest request_;
    ConnectionMock::Pointer dataserverConnection_;
    ConnectionMock::Pointer connection_;
    User user_;
};

TEST_F(GameserverCharacterCreateRequestTransactionTest, handle)
{
    Payload payload;
    EXPECT_CALL(*dataserverConnection_, send(_)).WillOnce(SaveArg<0>(&payload));
    EXPECT_CALL(*dataserverConnection_, isOpen()).WillOnce((Return(true)));
    user_.setAccountId(accountId_);

    eMU::gameserver::transactions::CharacterCreateRequest(user_, dataserverConnection_, request_).handle();

    ReadStream readStream(payload);
    ASSERT_EQ(streamIds::kCharacterCreateRequest, readStream.getId());

    CharacterCreateRequest characterCreateRequest(readStream);
    ASSERT_EQ(user_.getHash(), characterCreateRequest.getUserHash());
    ASSERT_EQ(user_.getAccountId(), characterCreateRequest.getAccountId());

    EXPECT_EQ(request_.getCharacterCreateInfo().faceScars_, characterCreateRequest.getCharacterCreateInfo().faceScars_);
    EXPECT_EQ(request_.getCharacterCreateInfo().face_, characterCreateRequest.getCharacterCreateInfo().face_);
    EXPECT_EQ(request_.getCharacterCreateInfo().hairColor_, characterCreateRequest.getCharacterCreateInfo().hairColor_);
    EXPECT_EQ(request_.getCharacterCreateInfo().hairType_, characterCreateRequest.getCharacterCreateInfo().hairType_);
    EXPECT_EQ(request_.getCharacterCreateInfo().name_, characterCreateRequest.getCharacterCreateInfo().name_);
    EXPECT_EQ(request_.getCharacterCreateInfo().race_, characterCreateRequest.getCharacterCreateInfo().race_);
    EXPECT_EQ(request_.getCharacterCreateInfo().skinColor_, characterCreateRequest.getCharacterCreateInfo().skinColor_);
    EXPECT_EQ(request_.getCharacterCreateInfo().skin_, characterCreateRequest.getCharacterCreateInfo().skin_);
    EXPECT_EQ(request_.getCharacterCreateInfo().tatoo_, characterCreateRequest.getCharacterCreateInfo().tatoo_);
}

TEST_F(GameserverCharacterCreateRequestTransactionTest, WhenConnectionToDataserverIsNotOpenThenClientShouldBeDisconnected)
{
    EXPECT_CALL(*dataserverConnection_, isOpen()).WillOnce((Return(false)));
    EXPECT_CALL(*connection_, disconnect());

    eMU::gameserver::transactions::CharacterCreateRequest(user_, dataserverConnection_, request_).handle();
}

TEST_F(GameserverCharacterCreateRequestTransactionTest, WhenNullPtrAsDataserverConnectionProvidedThenClientShouldBeDisconnected)
{
    EXPECT_CALL(*connection_, disconnect());

    eMU::gameserver::transactions::CharacterCreateRequest(user_, nullptr, request_).handle();
}

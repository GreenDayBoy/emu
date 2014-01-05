#include <gameserver/transactions/charactersListRequest.hpp>
#include <gameserver/user.hpp>

#include <streaming/readStream.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/gameserver/charactersListRequest.hpp>

#include <ut/env/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

using eMU::ut::env::core::network::tcp::ConnectionMock;
using eMU::core::network::Payload;
using eMU::streaming::ReadStream;
using eMU::gameserver::User;
using eMU::streaming::dataserver::CharactersListRequest;
namespace streamIds = eMU::streaming::dataserver::streamIds;

class CharactersListRequestTransactionTest: public ::testing::Test
{
protected:
    CharactersListRequestTransactionTest():
        accountId_("testtesttest"),
        request_(ReadStream(eMU::streaming::gameserver::CharactersListRequest().getWriteStream().getPayload())),
        dataserverConnection_(new ConnectionMock()),
        connection_(new ConnectionMock()),
        user_(connection_) {}

    std::string accountId_;
    eMU::streaming::gameserver::CharactersListRequest request_;
    ConnectionMock::Pointer dataserverConnection_;
    ConnectionMock::Pointer connection_;
    User user_;
};

TEST_F(CharactersListRequestTransactionTest, handle)
{
    Payload payload;
    EXPECT_CALL(*dataserverConnection_, send(_)).WillOnce(SaveArg<0>(&payload));
    EXPECT_CALL(*dataserverConnection_, isOpen()).WillOnce((Return(true)));
    user_.setAccountId(accountId_);

    eMU::gameserver::transactions::CharactersListRequest(user_, dataserverConnection_, request_).handle();

    ReadStream readStream(payload);
    ASSERT_EQ(streamIds::kCharactersListRequest, readStream.getId());

    CharactersListRequest charactersListRequest(readStream);
    ASSERT_EQ(user_.getHash(), charactersListRequest.getUserHash());
    ASSERT_EQ(user_.getAccountId(), charactersListRequest.getAccountId());
}

TEST_F(CharactersListRequestTransactionTest, WhenConnectionToDataserverIsNotOpenThenClientShouldBeDisconnected)
{
    EXPECT_CALL(*dataserverConnection_, isOpen()).WillOnce((Return(false)));
    EXPECT_CALL(*connection_, disconnect());

    eMU::gameserver::transactions::CharactersListRequest(user_, dataserverConnection_, request_).handle();
}

TEST_F(CharactersListRequestTransactionTest, WhenNullPtrAsDataserverConnectionProvidedThenClientShouldBeDisconnected)
{
    EXPECT_CALL(*connection_, disconnect());

    eMU::gameserver::transactions::CharactersListRequest(user_, nullptr, request_).handle();
}

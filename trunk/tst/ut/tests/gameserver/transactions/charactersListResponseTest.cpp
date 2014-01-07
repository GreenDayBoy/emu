#include <gameserver/transactions/charactersListResponse.hpp>
#include <gameserver/user.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <streaming/gameserver/charactersListResponse.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::SaveArg;

using eMU::gameserver::User;
using eMU::core::common::Factory;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::tcp::ConnectionMock;

using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::CharactersListResponse;
using eMU::streaming::common::CharacterInfoContainer;

using eMU::ut::env::core::network::tcp::ConnectionMock;

class CharactersListResponseTransactionTest: public ::testing::Test
{
protected:
    CharactersListResponseTransactionTest():
        connection_(new ConnectionMock()),
        characters_({{"andrew", 13, 8, 2, 3, 0},
                     {"george", 123, 4, 0, 5, 1}}) {}

    Factory<User> usersFactory_;
    ConnectionMock::Pointer connection_;
    CharacterInfoContainer characters_;
    Payload payload_;
};

TEST_F(CharactersListResponseTransactionTest, handle)
{
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    User &user = usersFactory_.create(connection_);

    eMU::gameserver::transactions::CharactersListResponse(usersFactory_,
                                                          CharactersListResponse(CharactersListResponse(user.getHash(), characters_).getWriteStream().getPayload())).handle();

    ReadStream charactersListResponseStream(payload_);
    ASSERT_EQ(eMU::streaming::gameserver::streamIds::kCharactersListResponse, charactersListResponseStream.getId());

    eMU::streaming::gameserver::CharactersListResponse charactersListResponse(charactersListResponseStream);
    ASSERT_EQ(characters_.size(), charactersListResponse.getCharacters().size());

    EXPECT_EQ(characters_[0].name_, charactersListResponse.getCharacters()[0].name_);
    EXPECT_EQ(characters_[0].level_, charactersListResponse.getCharacters()[0].level_);
    EXPECT_EQ(characters_[0].race_, charactersListResponse.getCharacters()[0].race_);
    EXPECT_EQ(characters_[0].hairType_, charactersListResponse.getCharacters()[0].hairType_);
    EXPECT_EQ(characters_[0].hairColor_, charactersListResponse.getCharacters()[0].hairColor_);
    EXPECT_EQ(characters_[0].tutorialState_, charactersListResponse.getCharacters()[0].tutorialState_);

    EXPECT_EQ(characters_[1].name_, charactersListResponse.getCharacters()[1].name_);
    EXPECT_EQ(characters_[1].level_, charactersListResponse.getCharacters()[1].level_);
    EXPECT_EQ(characters_[1].race_, charactersListResponse.getCharacters()[1].race_);
    EXPECT_EQ(characters_[1].hairType_, charactersListResponse.getCharacters()[1].hairType_);
    EXPECT_EQ(characters_[1].hairColor_, charactersListResponse.getCharacters()[1].hairColor_);
    EXPECT_EQ(characters_[1].tutorialState_, charactersListResponse.getCharacters()[1].tutorialState_);
}

TEST_F(CharactersListResponseTransactionTest, WhenHashGivenFromDataServerIsInvalidThenNothingShouldHappen)
{
    usersFactory_.create(connection_);

    eMU::gameserver::transactions::CharactersListResponse(usersFactory_,
                                                          CharactersListResponse(CharactersListResponse(User::Hash(0x12453), characters_).getWriteStream().getPayload())).handle();
}

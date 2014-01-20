#include <gameserver/transactions/characterCreateResponse.hpp>
#include <gameserver/user.hpp>
#include <streaming/gameserver/characterCreateResponse.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <ut/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::SaveArg;

using eMU::gameserver::User;
using eMU::core::common::Factory;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::tcp::ConnectionMock;

using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::CharacterCreateResponse;
using eMU::streaming::dataserver::CharacterCreateResult;

using eMU::ut::env::core::network::tcp::ConnectionMock;

class GameserverCharacterCreateResponseTransactionTest: public ::testing::Test
{
protected:
    GameserverCharacterCreateResponseTransactionTest():
        connection_(new ConnectionMock()) {}

    Factory<User> usersFactory_;
    ConnectionMock::Pointer connection_;
    Payload payload_;
};

TEST_F(GameserverCharacterCreateResponseTransactionTest, handle)
{
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload_));

    User &user = usersFactory_.create(connection_);

    eMU::gameserver::transactions::CharacterCreateResponse(usersFactory_,
                                                           CharacterCreateResponse(CharacterCreateResponse(user.getHash(),
                                                                                                            CharacterCreateResult::CharactersCountExceeded).getWriteStream().getPayload())).handle();

    ReadStream responseStream(payload_);
    ASSERT_EQ(eMU::streaming::gameserver::streamIds::kCharacterCreateResponse, responseStream.getId());

    eMU::streaming::gameserver::CharacterCreateResponse response(responseStream);
    EXPECT_EQ(eMU::streaming::gameserver::CharacterCreateResult::CharactersCountExceeded, response.getResult());
}

TEST_F(GameserverCharacterCreateResponseTransactionTest, WhenHashGivenFromDataServerIsInvalidThenNothingShouldHappen)
{
    usersFactory_.create(connection_);

    eMU::gameserver::transactions::CharacterCreateResponse(usersFactory_,
                                                           CharacterCreateResponse(CharacterCreateResponse(User::Hash(0x67826),
                                                                                                            CharacterCreateResult::CharactersCountExceeded).getWriteStream().getPayload())).handle();
}

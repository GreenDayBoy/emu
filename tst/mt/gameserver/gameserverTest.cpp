#include <gameserver/context.hpp>
#include <gameserver/protocol.hpp>
#include <gameserver/dataserverProtocol.hpp>
#include <gameserver/udpProtocol.hpp>
#include <core/network/tcp/connection.hpp>
#include <core/network/udp/connection.hpp>

#include <streaming/gameserver/registerUserRequest.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>
#include <streaming/gameserver/worldLoginRequest.hpp>
#include <streaming/gameserver/worldLoginResponse.hpp>
#include <streaming/gameserver/charactersListRequest.hpp>
#include <streaming/gameserver/charactersListResponse.hpp>
#include <streaming/gameserver/characterCreateRequest.hpp>
#include <streaming/gameserver/characterCreateResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>

#include <mt/asioStub/ioService.hpp>
#include <mt/check.hpp>

#include <gtest/gtest.h>

using eMU::gameserver::Context;
using eMU::gameserver::Protocol;
using eMU::gameserver::DataserverProtocol;
using eMU::gameserver::UdpProtocol;
using eMU::core::network::tcp::NetworkUser;
using eMU::mt::env::asioStub::io_service;
using eMU::streaming::ReadStream;
using eMU::streaming::gameserver::RegisterUserRequest;
using eMU::streaming::gameserver::RegisterUserResponse;
using eMU::streaming::gameserver::UserRegistrationResult;
using eMU::streaming::gameserver::WorldLoginRequest;
using eMU::streaming::gameserver::WorldLoginResponse;
using eMU::streaming::gameserver::CharactersListRequest;
using eMU::streaming::gameserver::CharactersListResponse;
using eMU::streaming::gameserver::CharacterCreateRequest;
using eMU::streaming::gameserver::CharacterCreateResponse;
using eMU::streaming::gameserver::CharacterCreateResult;
using eMU::streaming::dataserver::FaultIndication;
using eMU::streaming::common::CharacterInfoContainer;
using eMU::streaming::common::CharacterViewInfo;

class GameserverTest: public ::testing::Test
{
protected:
    GameserverTest():
        gameserverContext_(5, 30),
        gameserverProtocol_(gameserverContext_),
        dataserverProtocol_(gameserverContext_),
        udpProtocol_(gameserverContext_),
        connection_(new eMU::core::network::tcp::Connection(ioService_, gameserverProtocol_)) {}

    void SetUp()
    {
        prepareDataserverConnection();
        prepareUdpConnection();

        registerUserScenario(UserRegistrationResult::Succeed);
        connection_->accept();
        ASSERT_EQ(1, gameserverContext_.getUsersFactory().getObjects().size());
    }

    void prepareDataserverConnection()
    {
        eMU::core::network::tcp::Connection::Pointer dataserverConnection(new eMU::core::network::tcp::Connection(ioService_, dataserverProtocol_));
        dataserverConnection->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 55960));
        ASSERT_EQ(dataserverConnection, gameserverContext_.getClientConnection());
    }

    void prepareUdpConnection()
    {
        eMU::core::network::udp::Connection::Pointer udpConnection(new eMU::core::network::udp::Connection(ioService_, 55557, udpProtocol_));
        udpConnection->registerConnection();
        udpConnection->queueReceiveFrom();
        ASSERT_EQ(udpConnection, gameserverContext_.getUdpConnection());
    }

    void registerUserScenario(UserRegistrationResult result, NetworkUser::Hash userHash = NetworkUser::Hash(0x2371), const std::string &accountId = "testLogin")
    {
        IO_CHECK(gameserverContext_.getUdpConnection()->getSocket().send(RegisterUserRequest({userHash, accountId}).getWriteStream().getPayload()));

        ASSERT_TRUE(gameserverContext_.getUdpConnection()->getSocket().isUnread());
        const ReadStream &registerUserResponseStream = ReadStream(gameserverContext_.getUdpConnection()->getSocket().receive());
        ASSERT_EQ(eMU::streaming::gameserver::streamIds::kRegisterUserResponse, registerUserResponseStream.getId());

        RegisterUserResponse registerUserResponse(registerUserResponseStream);
        ASSERT_EQ(userHash, registerUserResponse.getUserHash());
        ASSERT_EQ(result, registerUserResponse.getResult());
        ASSERT_EQ(gameserverContext_.getGameserverCode(), registerUserResponse.getGameserverCode());
    }

    void faultIndicationScenario(bool userHashExists)
    {
        NetworkUser::Hash userHash = userHashExists ? gameserverContext_.getUsersFactory().getObjects().back()->getHash() : NetworkUser::Hash(0x1234);
        IO_CHECK(gameserverContext_.getClientConnection()->getSocket().send(FaultIndication(userHash, "test message").getWriteStream().getPayload()));

        bool connectionExists = !userHashExists;
        ASSERT_EQ(connectionExists, connection_->getSocket().is_open());
    }

    void TearDown()
    {
        ASSERT_FALSE(gameserverContext_.getClientConnection()->getSocket().isUnread());
        ASSERT_FALSE(connection_->getSocket().isUnread());
        ASSERT_FALSE(gameserverContext_.getUdpConnection()->getSocket().isUnread());

        connection_->disconnect();
        ASSERT_EQ(0, gameserverContext_.getUsersFactory().getObjects().size());

        gameserverContext_.getUdpConnection()->unregisterConnection();
    }

    Context gameserverContext_;
    Protocol gameserverProtocol_;
    DataserverProtocol dataserverProtocol_;
    UdpProtocol udpProtocol_;
    io_service ioService_;
    eMU::core::network::tcp::Connection::Pointer connection_;
};

TEST_F(GameserverTest, RegisterUser)
{
    registerUserScenario(UserRegistrationResult::Succeed);
}

TEST_F(GameserverTest, WhenRegistrationInfoWasNotProvidedThenConnectionShouldBeRejected)
{
    eMU::core::network::tcp::Connection::Pointer connection(new eMU::core::network::tcp::Connection(ioService_, gameserverProtocol_));
    connection->accept();

    ASSERT_FALSE(connection->getSocket().is_open());
}

TEST_F(GameserverTest, WhenSameRegistrationInfoWasProvidedTwiceThenUserRegistrationShouldFailed)
{
    NetworkUser::Hash userHash(0x3333);
    std::string accountId = "login";

    registerUserScenario(UserRegistrationResult::Succeed, userHash, accountId);
    registerUserScenario(UserRegistrationResult::Failed, userHash, accountId);
}

TEST_F(GameserverTest, WhenFaultIndicationReceivedThenClientShouldBeDisconnected)
{
    bool userHashExists = true;
    faultIndicationScenario(userHashExists);
}

TEST_F(GameserverTest, WhenFaultIndicationWithInvalidUserHashReceivedThenNothingHappens)
{
    bool userHashExists = false;
    faultIndicationScenario(userHashExists);
}

TEST_F(GameserverTest, WorldLogin)
{
    IO_CHECK(connection_->getSocket().send(WorldLoginRequest().getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &worldLoginResponseStream = ReadStream(connection_->getSocket().receive());
    ASSERT_EQ(eMU::streaming::gameserver::streamIds::kWorldLoginResponse, worldLoginResponseStream.getId());

    WorldLoginResponse worldLoginResponse(worldLoginResponseStream);
    ASSERT_EQ(0, worldLoginResponse.getResult());
}

TEST_F(GameserverTest, CharactersList)
{
    IO_CHECK(connection_->getSocket().send(CharactersListRequest().getWriteStream().getPayload()));

    ASSERT_TRUE(gameserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &charactersListRequestStream = ReadStream(gameserverContext_.getClientConnection()->getSocket().receive());
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCharactersListRequest, charactersListRequestStream.getId());

    eMU::streaming::dataserver::CharactersListRequest charactersListRequest(charactersListRequestStream);
    ASSERT_EQ(gameserverContext_.getUsersFactory().getObjects().back()->getHash(), charactersListRequest.getUserHash());
    ASSERT_EQ(gameserverContext_.getUsersFactory().getObjects().back()->getAccountId(), charactersListRequest.getAccountId());

    IO_CHECK(gameserverContext_.getClientConnection()->getSocket().send(eMU::streaming::dataserver::CharactersListResponse(charactersListRequest.getUserHash(),
                                                                                                                           CharacterInfoContainer()).getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &charactersListResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(eMU::streaming::gameserver::streamIds::kCharactersListResponse, charactersListResponseStream.getId());

    CharactersListResponse charactersListResponse(charactersListResponseStream);
    ASSERT_TRUE(charactersListResponse.getCharacters().empty());
}

TEST_F(GameserverTest, WhenCharactersListResponseReceivedWithInvalidUserHashThenNothingHappens)
{
    IO_CHECK(connection_->getSocket().send(CharactersListRequest().getWriteStream().getPayload()));

    ASSERT_TRUE(gameserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &charactersListRequestStream = gameserverContext_.getClientConnection()->getSocket().receive();
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCharactersListRequest, charactersListRequestStream.getId());

    IO_CHECK(gameserverContext_.getClientConnection()->getSocket().send(eMU::streaming::dataserver::CharactersListResponse(NetworkUser::Hash(0x1234),
                                                                                                                           CharacterInfoContainer()).getWriteStream().getPayload()));
    ASSERT_TRUE(connection_->getSocket().is_open());
}

TEST_F(GameserverTest, CharacterCreate)
{
    CharacterViewInfo characterCreateInfo("george", 5, 11, 122, 3, 7, 9, 53, 8);
    IO_CHECK(connection_->getSocket().send(CharacterCreateRequest(characterCreateInfo).getWriteStream().getPayload()));

    ASSERT_TRUE(gameserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &characterCreateRequestStream = ReadStream(gameserverContext_.getClientConnection()->getSocket().receive());
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCharacterCreateRequest, characterCreateRequestStream.getId());

    eMU::streaming::dataserver::CharacterCreateRequest characterCreateRequest(characterCreateRequestStream);
    ASSERT_EQ(gameserverContext_.getUsersFactory().getObjects().back()->getHash(), characterCreateRequest.getUserHash());
    ASSERT_EQ(gameserverContext_.getUsersFactory().getObjects().back()->getAccountId(), characterCreateRequest.getAccountId());
    ASSERT_EQ(characterCreateInfo.faceScars_, characterCreateRequest.getCharacterCreateInfo().faceScars_);
    ASSERT_EQ(characterCreateInfo.face_, characterCreateRequest.getCharacterCreateInfo().face_);
    ASSERT_EQ(characterCreateInfo.hairColor_, characterCreateRequest.getCharacterCreateInfo().hairColor_);
    ASSERT_EQ(characterCreateInfo.hairType_, characterCreateRequest.getCharacterCreateInfo().hairType_);
    ASSERT_EQ(characterCreateInfo.name_, characterCreateRequest.getCharacterCreateInfo().name_);
    ASSERT_EQ(characterCreateInfo.race_, characterCreateRequest.getCharacterCreateInfo().race_);
    ASSERT_EQ(characterCreateInfo.skinColor_, characterCreateRequest.getCharacterCreateInfo().skinColor_);
    ASSERT_EQ(characterCreateInfo.skin_, characterCreateRequest.getCharacterCreateInfo().skin_);
    ASSERT_EQ(characterCreateInfo.tatoo_, characterCreateRequest.getCharacterCreateInfo().tatoo_);

    IO_CHECK(gameserverContext_.getClientConnection()->getSocket().send(eMU::streaming::dataserver::CharacterCreateResponse(characterCreateRequest.getUserHash(),
                                                                                                                            eMU::streaming::dataserver::CharacterCreateResult::AlreadyExists).getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &characterCreateResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(eMU::streaming::gameserver::streamIds::kCharacterCreateResponse, characterCreateResponseStream.getId());

    CharacterCreateResponse characterCreateResponse(characterCreateResponseStream);
    ASSERT_EQ(CharacterCreateResult::AlreadyExists, characterCreateResponse.getResult());
}

TEST_F(GameserverTest, WhenCharacterCreateResponseReceivedWithInvalidUserHashThenNothingHappens)
{
    IO_CHECK(connection_->getSocket().send(CharacterCreateRequest(CharacterViewInfo("andrew", 1, 2, 3, 4, 5, 6, 7, 8)).getWriteStream().getPayload()));

    ASSERT_TRUE(gameserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &characterCreateRequestStream = gameserverContext_.getClientConnection()->getSocket().receive();
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCharacterCreateRequest, characterCreateRequestStream.getId());

    eMU::streaming::dataserver::CharacterCreateResponse response(NetworkUser::Hash(0x1234), eMU::streaming::dataserver::CharacterCreateResult::Succeed);
    IO_CHECK(gameserverContext_.getClientConnection()->getSocket().send(response.getWriteStream().getPayload()));
    ASSERT_TRUE(connection_->getSocket().is_open());
}

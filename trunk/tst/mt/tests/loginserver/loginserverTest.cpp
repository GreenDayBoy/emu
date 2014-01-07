#include <loginserver/context.hpp>
#include <loginserver/protocol.hpp>
#include <loginserver/dataserverProtocol.hpp>
#include <loginserver/udpProtocol.hpp>
#include <core/common/xmlReader.hpp>
#include <core/network/tcp/connection.hpp>
#include <core/network/udp/connection.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/check.hpp>
#include <streaming/loginserver/streamIds.hpp>
#include <streaming/loginserver/loginRequest.hpp>
#include <streaming/loginserver/loginResponse.hpp>
#include <streaming/loginserver/loginResult.hpp>
#include <streaming/loginserver/gameserversListRequest.hpp>
#include <streaming/loginserver/gameserversListResponse.hpp>
#include <streaming/loginserver/gameserverDetailsRequest.hpp>
#include <streaming/loginserver/gameserverDetailsResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>

#include <gtest/gtest.h>

using eMU::loginserver::Context;
using eMU::loginserver::Protocol;
using eMU::loginserver::DataserverProtocol;
using eMU::loginserver::UdpProtocol;
using eMU::core::common::XmlReader;
using eMU::core::network::tcp::NetworkUser;
using eMU::mt::env::asioStub::io_service;
using eMU::streaming::ReadStream;
using eMU::streaming::loginserver::LoginRequest;
using eMU::streaming::loginserver::LoginResponse;
using eMU::streaming::loginserver::LoginResult;
using eMU::streaming::loginserver::GameserversListRequest;
using eMU::streaming::loginserver::GameserversListResponse;
using eMU::streaming::loginserver::GameserverDetailsRequest;
using eMU::streaming::loginserver::GameserverDetailsResponse;
namespace streamIds = eMU::streaming::loginserver::streamIds;
using eMU::streaming::dataserver::CheckAccountRequest;
using eMU::streaming::dataserver::CheckAccountResponse;
using eMU::streaming::dataserver::CheckAccountResult;
using eMU::streaming::dataserver::FaultIndication;
using eMU::streaming::gameserver::RegisterUserRequest;
using eMU::streaming::gameserver::RegisterUserResponse;
using eMU::streaming::gameserver::UserRegistrationResult;

class LoginserverTest: public ::testing::Test
{
protected:
    LoginserverTest():
        loginserverContext_(5),
        loginserverProtocol_(loginserverContext_),
        connection_(new eMU::core::network::tcp::Connection(ioService_, loginserverProtocol_)),
        dataserverProtocol_(loginserverContext_),
        udpProtocol_(loginserverContext_),
        gameserverCode_(0) {}

    void SetUp()
    {
        initializeGameserversList();
        prepareDataserverConnection();
        prepareUdpConnection();

        connection_->accept();
        ASSERT_EQ(1, loginserverContext_.getUsersFactory().getObjects().size());
    }

    void initializeGameserversList()
    {
        const std::string &xmlContent = "<servers> \
                                            <server code=\"0\" name=\"eMU_TEST1\" address=\"192.168.0.1\" port=\"55901\" /> \
                                            <server code=\"1\" name=\"eMU_TEST2\" address=\"127.0.0.1\" port=\"55902\" /> \
                                        </servers>";

        XmlReader xmlReader(xmlContent);
        ASSERT_TRUE(loginserverContext_.getGameserversList().initialize(xmlReader));
    }

    void prepareDataserverConnection()
    {
        eMU::core::network::tcp::Connection::Pointer dataserverConnection(new eMU::core::network::tcp::Connection(ioService_, dataserverProtocol_));
        dataserverConnection->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 55960));
        ASSERT_EQ(dataserverConnection, loginserverContext_.getClientConnection());
    }

    void prepareUdpConnection()
    {
        eMU::core::network::udp::Connection::Pointer udpConnection(new eMU::core::network::udp::Connection(ioService_, 55557, udpProtocol_));
        udpConnection->registerConnection();
        udpConnection->queueReceiveFrom();
        ASSERT_EQ(udpConnection, loginserverContext_.getUdpConnection());
    }

    void loginScenario()
    {
        LoginRequest loginRequest(L"accountTest", L"passwordTest");
        IO_CHECK(connection_->getSocket().send(loginRequest.getWriteStream().getPayload()));

        ASSERT_TRUE(loginserverContext_.getClientConnection()->getSocket().isUnread());
        const ReadStream &checkAccountRequestStream = loginserverContext_.getClientConnection()->getSocket().receive();
        ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCheckAccountRequest, checkAccountRequestStream.getId());

        CheckAccountRequest checkAccountRequest(checkAccountRequestStream);
        ASSERT_EQ("accountTest", checkAccountRequest.getAccountId());
        ASSERT_EQ("passwordTest", checkAccountRequest.getPassword());

        IO_CHECK(loginserverContext_.getClientConnection()->getSocket().send(CheckAccountResponse(checkAccountRequest.getUserHash(),
                                                                                                   CheckAccountResult::Succeed).getWriteStream().getPayload()));
        ASSERT_TRUE(connection_->getSocket().isUnread());
        const ReadStream &loginResponseStream = connection_->getSocket().receive();
        ASSERT_EQ(streamIds::kLoginResponse, loginResponseStream.getId());

        LoginResponse loginResponse(loginResponseStream);
        ASSERT_EQ(LoginResult::Succeed, loginResponse.getResult());
    }

    void gameserverDetailsScenario(NetworkUser::Hash userHash, UserRegistrationResult result)
    {
        loginScenario();

        IO_CHECK(connection_->getSocket().send(GameserverDetailsRequest(gameserverCode_).getWriteStream().getPayload()));

        ASSERT_TRUE(loginserverContext_.getUdpConnection()->getSocket().isUnread());
        const ReadStream &registerUserRequestStream = loginserverContext_.getUdpConnection()->getSocket().receive();
        ASSERT_EQ(eMU::streaming::gameserver::streamIds::kRegisterUserRequest, registerUserRequestStream.getId());

        RegisterUserRequest registerUserRequest(registerUserRequestStream);
        ASSERT_EQ("accountTest", registerUserRequest.getUserRegistrationInfo().accountId_);
        ASSERT_EQ(loginserverContext_.getUsersFactory().getObjects().back()->getHash(), registerUserRequest.getUserRegistrationInfo().userHash_);

        IO_CHECK(loginserverContext_.getUdpConnection()->getSocket().send(RegisterUserResponse(gameserverCode_,
                                                                                               userHash,
                                                                                               result).getWriteStream().getPayload()));
    }

    void faultIndicationScenario(bool userHashExists)
    {
        NetworkUser::Hash userHash = userHashExists ? loginserverContext_.getUsersFactory().getObjects().back()->getHash() : NetworkUser::Hash(0x1234);
        IO_CHECK(loginserverContext_.getClientConnection()->getSocket().send(FaultIndication(userHash, "test message").getWriteStream().getPayload()));

        bool connectionExists = !userHashExists;
        ASSERT_EQ(connectionExists, connection_->getSocket().is_open());
    }

    void TearDown()
    {
        ASSERT_FALSE(loginserverContext_.getClientConnection()->getSocket().isUnread());
        ASSERT_FALSE(connection_->getSocket().isUnread());
        ASSERT_FALSE(loginserverContext_.getUdpConnection()->getSocket().isUnread());

        connection_->getSocket().disconnect();
        ASSERT_EQ(0, loginserverContext_.getUsersFactory().getObjects().size());

        loginserverContext_.getUdpConnection()->unregisterConnection();
    }

    Context loginserverContext_;
    Protocol loginserverProtocol_;
    io_service ioService_;
    eMU::core::network::tcp::Connection::Pointer connection_;
    DataserverProtocol dataserverProtocol_;
    UdpProtocol udpProtocol_;
    uint16_t gameserverCode_;
};

TEST_F(LoginserverTest, Login)
{
    loginScenario();
}

TEST_F(LoginserverTest, WhenCheckAccountResponseReceivedWithInvalidUserHashThenNothingHappens)
{
    LoginRequest loginRequest(L"accountTest", L"passwordTest");
    IO_CHECK(connection_->getSocket().send(loginRequest.getWriteStream().getPayload()));

    ASSERT_TRUE(loginserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &checkAccountRequestStream = loginserverContext_.getClientConnection()->getSocket().receive();
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCheckAccountRequest, checkAccountRequestStream.getId());

    IO_CHECK(loginserverContext_.getClientConnection()->getSocket().send(CheckAccountResponse(NetworkUser::Hash(0x1234),
                                                                                               CheckAccountResult::Succeed).getWriteStream().getPayload()));
    ASSERT_TRUE(connection_->getSocket().is_open());
}

TEST_F(LoginserverTest, WhenFaultIndicationReceivedThenClientShouldBeDisconnected)
{
    bool userHashExists = true;
    faultIndicationScenario(userHashExists);
}

TEST_F(LoginserverTest, WhenFaultIndicationWithInvalidUserHashReceivedThenNothingHappens)
{
    bool userHashExists = false;
    faultIndicationScenario(userHashExists);
}

TEST_F(LoginserverTest, GameserversList)
{
    IO_CHECK(connection_->getSocket().send(GameserversListRequest().getWriteStream().getPayload()));

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &gameserversListResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(eMU::streaming::loginserver::streamIds::kGameserversListResponse, gameserversListResponseStream.getId());

    GameserversListResponse response(gameserversListResponseStream);

    ASSERT_EQ(2, response.getServers().size());

    ASSERT_EQ(0, response.getServers()[0].code_);
    ASSERT_EQ("eMU_TEST1", response.getServers()[0].name_);

    ASSERT_EQ(1, response.getServers()[1].code_);
    ASSERT_EQ("eMU_TEST2", response.getServers()[1].name_);
}

TEST_F(LoginserverTest, GameserverDetails)
{
    gameserverDetailsScenario(loginserverContext_.getUsersFactory().getObjects().back()->getHash(), UserRegistrationResult::Succeed);

    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &gameserverDetailsResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(streamIds::kGameserverDetailsResponse, gameserverDetailsResponseStream.getId());

    GameserverDetailsResponse gameserverDetailsResponse(gameserverDetailsResponseStream);
    ASSERT_EQ("192.168.0.1", gameserverDetailsResponse.getIpAddress());
    ASSERT_EQ(55901, gameserverDetailsResponse.getPort());
}

TEST_F(LoginserverTest, WhenUserRegistrationResultIsFailedThenConnectionShouldBeDisconnected)
{
    gameserverDetailsScenario(loginserverContext_.getUsersFactory().getObjects().back()->getHash(), UserRegistrationResult::Failed);

    ASSERT_FALSE(connection_->isOpen());
    ASSERT_EQ(0, loginserverContext_.getUsersFactory().getObjects().size());
}

TEST_F(LoginserverTest, WhenUserRegistrationResultProvidedInvalidUserHashThenNothingHappens)
{
    gameserverDetailsScenario(NetworkUser::Hash(0x5317), UserRegistrationResult::Failed);

    ASSERT_TRUE(connection_->isOpen());
    ASSERT_EQ(1, loginserverContext_.getUsersFactory().getObjects().size());
}

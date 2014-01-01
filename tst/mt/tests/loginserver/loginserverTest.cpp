#include <loginserver/context.hpp>
#include <loginserver/protocol.hpp>
#include <loginserver/dataserverProtocol.hpp>
#include <core/common/xmlReader.hpp>
#include <core/network/tcp/connection.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/check.hpp>
#include <streaming/loginserver/streamIds.hpp>
#include <streaming/loginserver/loginRequest.hpp>
#include <streaming/loginserver/loginResponse.hpp>
#include <streaming/loginserver/loginResult.hpp>
#include <streaming/loginserver/gameserversListRequest.hpp>
#include <streaming/loginserver/gameserversListResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>
#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/dataserver/faultIndication.hpp>

#include <gtest/gtest.h>

using eMU::loginserver::Context;
using eMU::loginserver::Protocol;
using eMU::loginserver::DataserverProtocol;
using eMU::core::common::XmlReader;
using eMU::core::network::tcp::Connection;
using eMU::core::network::tcp::NetworkUser;
using eMU::mt::env::asioStub::io_service;
using eMU::streaming::ReadStream;
using eMU::streaming::loginserver::LoginRequest;
using eMU::streaming::loginserver::LoginResponse;
using eMU::streaming::loginserver::LoginResult;
using eMU::streaming::loginserver::GameserversListRequest;
using eMU::streaming::loginserver::GameserversListResponse;
namespace streamIds = eMU::streaming::loginserver::streamIds;
using eMU::streaming::dataserver::CheckAccountRequest;
using eMU::streaming::dataserver::CheckAccountResponse;
using eMU::streaming::dataserver::CheckAccountResult;
using eMU::streaming::dataserver::FaultIndication;

class LoginserverTest: public ::testing::Test
{
protected:
    LoginserverTest():
        loginserverContext_(5),
        loginserverProtocol_(loginserverContext_),
        connection_(new Connection(ioService_, loginserverProtocol_)),
        dataserverProtocol_(loginserverContext_) {}

    void SetUp()
    {
        initializeGameserversList();
        prepareDataserverConnection();

        connection_->accept();
        ASSERT_EQ(1, loginserverContext_.getUsersFactory().getObjects().size());
    }

    void initializeGameserversList()
    {
        const std::string &xmlContent = "<servers> \
                                            <server code=\"0\" name=\"eMU_TEST1\" address=\"localhost\" port=\"55901\" /> \
                                            <server code=\"1\" name=\"eMU_TEST2\" address=\"127.0.0.1\" port=\"55902\" /> \
                                        </servers>";

        XmlReader xmlReader(xmlContent);
        ASSERT_TRUE(loginserverContext_.getGameserversList().initialize(xmlReader));
    }

    void prepareDataserverConnection()
    {
        Connection::Pointer dataserverConnection(new Connection(ioService_, dataserverProtocol_));
        dataserverConnection->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 55960));
        ASSERT_EQ(dataserverConnection, loginserverContext_.getClientConnection());
    }

    void faultIndicationScenario(bool clientHashExists)
    {
        LoginRequest loginRequest(L"accountTest", L"passwordTest");
        IO_CHECK(connection_->getSocket().send(loginRequest.getWriteStream().getPayload()));

        ASSERT_TRUE(loginserverContext_.getClientConnection()->getSocket().isUnread());
        const ReadStream &checkAccountRequestStream = loginserverContext_.getClientConnection()->getSocket().receive();
        ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCheckAccountRequest, checkAccountRequestStream.getId());

        CheckAccountRequest checkAccountRequest(checkAccountRequestStream);
        NetworkUser::Hash clientHash = clientHashExists ? checkAccountRequest.getClientHash() : NetworkUser::Hash(0x1234);
        IO_CHECK(loginserverContext_.getClientConnection()->getSocket().send(FaultIndication(clientHash, "test message").getWriteStream().getPayload()));

        bool connectionExists = !clientHashExists;
        ASSERT_EQ(connectionExists, connection_->getSocket().is_open());
    }

    void TearDown()
    {
        ASSERT_FALSE(loginserverContext_.getClientConnection()->getSocket().isUnread());
        ASSERT_FALSE(connection_->getSocket().isUnread());

        connection_->getSocket().disconnect();
        ASSERT_EQ(0, loginserverContext_.getUsersFactory().getObjects().size());
    }

    Context loginserverContext_;
    Protocol loginserverProtocol_;
    io_service ioService_;
    Connection::Pointer connection_;
    DataserverProtocol dataserverProtocol_;
};

TEST_F(LoginserverTest, Login)
{
    LoginRequest loginRequest(L"accountTest", L"passwordTest");
    IO_CHECK(connection_->getSocket().send(loginRequest.getWriteStream().getPayload()));

    ASSERT_TRUE(loginserverContext_.getClientConnection()->getSocket().isUnread());
    const ReadStream &checkAccountRequestStream = loginserverContext_.getClientConnection()->getSocket().receive();
    ASSERT_EQ(eMU::streaming::dataserver::streamIds::kCheckAccountRequest, checkAccountRequestStream.getId());

    CheckAccountRequest checkAccountRequest(checkAccountRequestStream);
    ASSERT_EQ("accountTest", checkAccountRequest.getAccountId());
    ASSERT_EQ("passwordTest", checkAccountRequest.getPassword());

    IO_CHECK(loginserverContext_.getClientConnection()->getSocket().send(CheckAccountResponse(checkAccountRequest.getClientHash(),
                                                                                               CheckAccountResult::Succeed).getWriteStream().getPayload()));
    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &loginResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(streamIds::kLoginResponse, loginResponseStream.getId());

    LoginResponse loginResponse(loginResponseStream);
    ASSERT_EQ(LoginResult::Succeed, loginResponse.getResult());
}

TEST_F(LoginserverTest, WhenCheckAccountWithInvalidClientHashReceivedThenNothingHappens)
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
    bool clientHashExists = true;
    faultIndicationScenario(clientHashExists);
}

TEST_F(LoginserverTest, WhenFaultIndicationWithInvalidClientHashReceivedThenNothingHappens)
{
    bool clientHashExists = false;
    faultIndicationScenario(clientHashExists);
}

TEST_F(LoginserverTest, checkGameserversListRequest)
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

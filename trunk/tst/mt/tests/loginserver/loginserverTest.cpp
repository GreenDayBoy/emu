#include <loginserver/context.hpp>
#include <loginserver/protocol.hpp>
#include <loginserver/dataserverProtocol.hpp>
#include <core/common/xmlReader.hpp>
#include <core/network/tcp/connection.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/check.hpp>

#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/loginRequest.hpp>
#include <protocol/loginserver/loginResponse.hpp>
#include <protocol/loginserver/loginResult.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>

#include <gtest/gtest.h>

using eMU::loginserver::Context;
using eMU::loginserver::Protocol;
using eMU::loginserver::DataserverProtocol;
using eMU::core::common::XmlReader;
using eMU::core::network::tcp::Connection;
using eMU::mt::env::asioStub::io_service;
using eMU::protocol::ReadStream;
using eMU::protocol::loginserver::LoginRequest;
using eMU::protocol::loginserver::LoginResponse;
using eMU::protocol::loginserver::LoginResult;
namespace MessageIds = eMU::protocol::loginserver::MessageIds;
using eMU::protocol::dataserver::CheckAccountRequest;
using eMU::protocol::dataserver::CheckAccountResponse;
using eMU::protocol::dataserver::CheckAccountResult;

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
        ASSERT_EQ(dataserverConnection, loginserverContext_.getDataserverConnection());
    }

    void TearDown()
    {
        ASSERT_FALSE(loginserverContext_.getDataserverConnection()->getSocket().isUnread());
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
    CHECK(connection_->getSocket().send(loginRequest.getWriteStream().getPayload()));

    ASSERT_TRUE(loginserverContext_.getDataserverConnection()->getSocket().isUnread());
    const ReadStream &checkAccountRequestStream = loginserverContext_.getDataserverConnection()->getSocket().receive();
    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountRequest, checkAccountRequestStream.getId());

    CheckAccountRequest checkAccountRequest(checkAccountRequestStream);
    ASSERT_EQ("accountTest", checkAccountRequest.getAccountId());
    ASSERT_EQ("passwordTest", checkAccountRequest.getPassword());

    CHECK(loginserverContext_.getDataserverConnection()->getSocket().send(CheckAccountResponse(checkAccountRequest.getClientHash(),
                                                                                               CheckAccountResult::Succeed).getWriteStream().getPayload()));
    ASSERT_TRUE(connection_->getSocket().isUnread());
    const ReadStream &loginResponseStream = connection_->getSocket().receive();
    ASSERT_EQ(MessageIds::kLoginResponse, loginResponseStream.getId());

    LoginResponse loginResponse(loginResponseStream);
    ASSERT_EQ(LoginResult::Succeed, loginResponse.getResult());
}

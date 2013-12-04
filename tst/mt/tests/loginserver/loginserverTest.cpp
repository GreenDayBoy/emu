#include <loginserver/server.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/testExceptionsCatch.hpp>

#include <protocol/loginserver/loginRequest.hpp>
#include <protocol/loginserver/loginResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/faultIndication.hpp>

using eMU::loginserver::Server;
using eMU::mt::env::asioStub::io_service;
using eMU::protocol::ReadStream;
using eMU::protocol::loginserver::LoginRequest;
using eMU::protocol::loginserver::LoginResponse;
using eMU::protocol::loginserver::LoginResult;
namespace MessageIds = eMU::protocol::loginserver::MessageIds;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::CheckAccountRequest;
using eMU::protocol::dataserver::CheckAccountResponse;
using eMU::protocol::dataserver::FaultIndication;

class LoginserverTest: public ::testing::Test
{
protected:
    LoginserverTest():
        server_(ioService_, configuration_)
    {
        prepareConfiguration();
    }

    void prepareConfiguration()
    {
        configuration_.maxNumberOfUsers_ = 5;
        configuration_.port_ = 55557;
        configuration_.dataserver1Address_ = "127.0.0.1";
        configuration_.dataserver1Port_ = 55960;
        configuration_.dataserver2Address_ = "127.0.0.1";
        configuration_.dataserver2Port_ = 55962;
        configuration_.gameserversListFileContent_ =         "<servers> \
                <server code=\"0\" name=\"eMU_Test\" address=\"localhost\" port=\"55901\" /> \
                <server code=\"20\" name=\"eMU_Test2\" address=\"127.0.0.1\" port=\"55902\" /> \
            </servers>";
    }

    void faultIndicationScenario(bool clientHashExists)
    {
        Server server(ioService_, configuration_);
        server.startup();

        size_t connectionHash = ioService_.establishTcpConnection();
        LoginRequest loginRequest(L"accountTest", L"passwordTest");
        IO_CHECK(ioService_.send(connectionHash, loginRequest.getWriteStream()));

        const ReadStream &checkAccountRequestStream = ioService_.receiveFromClientTcpSocket();
        ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountRequest, checkAccountRequestStream.getId());
        CheckAccountRequest checkAccountRequest(checkAccountRequestStream);

        size_t clientHash = clientHashExists ? checkAccountRequest.getClientHash() : 0x1234;
        IO_CHECK(ioService_.sendToClientTcpSocket(FaultIndication(clientHash, "test message").getWriteStream()));

        bool connectionShouldExists = clientHashExists ? false : true;
        ASSERT_EQ(connectionShouldExists, ioService_.tcpConnectionEstablished(connectionHash));
    }

    io_service ioService_;
    Server::Configuration configuration_;
    Server server_;
};

TEST_F(LoginserverTest, Login)
{
    Server server(ioService_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.establishTcpConnection();
    LoginRequest loginRequest(L"accountTest", L"passwordTest");
    IO_CHECK(ioService_.send(connectionHash, loginRequest.getWriteStream()));

    const ReadStream &checkAccountRequestStream = ioService_.receiveFromClientTcpSocket();
    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountRequest, checkAccountRequestStream.getId());

    CheckAccountRequest checkAccountRequest(checkAccountRequestStream);
    ASSERT_EQ("accountTest", checkAccountRequest.getAccountId());
    ASSERT_EQ("passwordTest", checkAccountRequest.getPassword());

    IO_CHECK(ioService_.sendToClientTcpSocket(CheckAccountResponse(checkAccountRequest.getClientHash(), CheckAccountResult::Succeed).getWriteStream()));

    const ReadStream &loginResponseStream = ioService_.receive(connectionHash);
    ASSERT_EQ(MessageIds::kLoginResponse, loginResponseStream.getId());

    LoginResponse loginResponse(loginResponseStream);
    ASSERT_EQ(LoginResult::Succeed, loginResponse.getResult());
}

TEST_F(LoginserverTest, WhenCheckAccountWithInvalidClientHashReceivedThenNothingHappens)
{
    Server server(ioService_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.establishTcpConnection();
    LoginRequest loginRequest(L"accountTest", L"passwordTest");
    IO_CHECK(ioService_.send(connectionHash, loginRequest.getWriteStream()));

    const ReadStream &checkAccountRequestStream = ioService_.receiveFromClientTcpSocket();
    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountRequest, checkAccountRequestStream.getId());

    IO_CHECK(ioService_.sendToClientTcpSocket(CheckAccountResponse(0x1234, CheckAccountResult::Succeed).getWriteStream()));
    ASSERT_TRUE(ioService_.tcpConnectionEstablished(connectionHash));
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

TEST_F(LoginserverTest, WhenConnectToDataserverFailedThenStartupShouldBeFailed)
{
    ioService_.setConnectResult(false);

    Server server(ioService_, configuration_);
    ASSERT_FALSE(server.startup());
}

TEST_F(LoginserverTest, WhenConnectToDataserverFailedThenClientShouldBeDisconnected)
{
    ioService_.setConnectResult(false);
    Server server(ioService_, configuration_);
    server.startup();

    size_t connectionHash = ioService_.establishTcpConnection();
    IO_CHECK(ioService_.send(connectionHash, LoginRequest(L"accountTest", L"passwordTest").getWriteStream()));

    ASSERT_FALSE(ioService_.tcpConnectionEstablished(connectionHash));
}

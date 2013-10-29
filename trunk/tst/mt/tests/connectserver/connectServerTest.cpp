#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mt/env/testExceptionsCatch.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/messages/builders/gameServerLoadIndicationBuilder.hpp>
#include <mt/env/messages/builders/gameServersListRequestBuilder.hpp>
#include <mt/env/messages/verifiers/gameServersListResponseVerifier.hpp>
#include <mt/env/messages/builders/gameServerAddressRequestBuilder.hpp>
#include <mt/env/messages/verifiers/gameServerAddressResponseVerifier.hpp>
#include <mt/env/messages/verifiers/handshakeIndicationVerifier.hpp>
#include <connectserver/server.hpp>

namespace mtEnv = eMU::mt::env;
namespace asioStub = mtEnv::asioStub;
namespace builders = mtEnv::messages::builders;
namespace verifiers = mtEnv::messages::verifiers;

class ConnectServerTest: public ::testing::Test
{
public:
    ConnectServerTest()
    {
        prepareConfiguration();
    }

    void prepareConfiguration()
    {
        configuration_.port_ = 44405;
        configuration_.maxNumberOfUsers_ = 3;
        configuration_.gameServersListContent_ = "<servers> \
                                                      <server code=\"0\" name=\"eMU_Test\" address=\"192.168.0.1\" port=\"55901\" /> \
                                                      <server code=\"20\" name=\"eMU_Test2\" address=\"127.0.0.1\" port=\"55902\" /> \
                                                  </servers>";
    }

    void checkInvalidMessageScenario(const eMU::core::network::Payload &payload)
    {
        eMU::connectserver::Server server(ioService_, configuration_);
        server.startup();

        size_t hash = ioService_.createConnection();

        verifiers::HandshakeIndicationVerifier()(ioService_.receive(hash));

        ioService_.send(hash, payload);

        ASSERT_FALSE(ioService_.exists(hash));
    }

    asioStub::io_service ioService_;
    eMU::connectserver::Server::Configuration configuration_;
};

TEST_F(ConnectServerTest, GameServerLoadUpdate_Check)
{
    try
    {

    eMU::connectserver::Server server(ioService_, configuration_);
    server.startup();

    uint16_t firstServerCode = 0;
    uint8_t firstServerLoad = 30;
    ioService_.sendTo(builders::GameServerLoadIndicationBuilder()(firstServerCode, firstServerLoad));

    uint16_t secondServerCode = 20;
    uint8_t secondServerLoad = 5;
    ioService_.sendTo(builders::GameServerLoadIndicationBuilder()(secondServerCode, secondServerLoad));

    size_t hash = ioService_.createConnection();

    verifiers::HandshakeIndicationVerifier()(ioService_.receive(hash));

    ioService_.send(hash, builders::GameServersListRequestBuilder()());

    verifiers::GameServersListResponseVerifier()(ioService_.receive(hash), {{firstServerCode, firstServerLoad, 0},
                                                                            {secondServerCode, secondServerLoad, 0}});

    ioService_.disconnect(hash);

    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, GameServerLoadUpdate_WhenServerCodeIsInvalidThenConnections)
{
    try
    {

    eMU::connectserver::Server server(ioService_, configuration_);
    server.startup();

    ioService_.sendTo(builders::GameServerLoadIndicationBuilder()(21, 55));

    uint16_t secondServerCode = 20;
    uint8_t secondServerLoad = 5;
    ioService_.sendTo(builders::GameServerLoadIndicationBuilder()(secondServerCode, secondServerLoad));

    size_t hash = ioService_.createConnection();

    verifiers::HandshakeIndicationVerifier()(ioService_.receive(hash));

    ioService_.send(hash, builders::GameServersListRequestBuilder()());

    verifiers::GameServersListResponseVerifier()(ioService_.receive(hash), {{0, 0, 0},
                                                                            {secondServerCode, secondServerLoad, 0}});

    ioService_.disconnect(hash);

    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, GetGameServerAddress_Check)
{
    try
    {

    eMU::connectserver::Server server(ioService_, configuration_);
    server.startup();

    size_t hash = ioService_.createConnection();

    verifiers::HandshakeIndicationVerifier()(ioService_.receive(hash));

    ioService_.send(hash, builders::GameServerAddressRequestBuilder()(0));
    verifiers::GameServerAddressResponseVerifier()(ioService_.receive(hash), "192.168.0.1", 55901);

    ioService_.send(hash, builders::GameServerAddressRequestBuilder()(20));
    verifiers::GameServerAddressResponseVerifier()(ioService_.receive(hash), "127.0.0.1", 55902);

    ioService_.disconnect(hash);

    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, GetGameServerAddress_WhenServerCodeIsInvalidThenConnectionShouldBeClosed)
{
    try
    {

    eMU::connectserver::Server server(ioService_, configuration_);
    server.startup();

    size_t hash = ioService_.createConnection();

    verifiers::HandshakeIndicationVerifier()(ioService_.receive(hash));

    ioService_.send(hash, builders::GameServerAddressRequestBuilder()(23));

    ASSERT_FALSE(ioService_.exists(hash));

    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, WhenMessageWithInvalidHeaderIdWasReceivedThenConnectionShouldBeClosed)
{
    try
    {
        checkInvalidMessageScenario({0xC5, 0x04, 0x00, 0x01});
    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, WhenMessageWithInvalidMessageIdWasReceivedThenConnectionShouldBeClosed)
{
    try
    {
        checkInvalidMessageScenario({0xC1, 0x04, 0x00, 0xF5});
    }
    TEST_EXCEPTIONS_CATCH
}

TEST_F(ConnectServerTest, WhenMessageWithInvalidSizeWasReceivedThenConnectionShouldBeClosed)
{
    try
    {
        checkInvalidMessageScenario({0xC1, 0x00, 0x00, 0xF4});
    }
    TEST_EXCEPTIONS_CATCH
}

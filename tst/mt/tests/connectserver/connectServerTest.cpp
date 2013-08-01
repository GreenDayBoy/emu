#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/exceptions.hpp>
#include <mt/env/exceptions.hpp>
#include <mt/env/messages/builders/gameServerLoadIndicationBuilder.hpp>
#include <mt/env/messages/builders/gameServersListRequestBuilder.hpp>
#include <mt/env/messages/verifiers/gameServersListResponseVerifier.hpp>
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
                                                      <server code=\"0\" name=\"eMU_Test\" address=\"localhost\" port=\"55901\" /> \
                                                      <server code=\"20\" name=\"eMU_Test2\" address=\"127.0.0.1\" port=\"55902\" /> \
                                                  </servers>";
    }

    asioStub::io_service ioService_;
    eMU::connectserver::Server::Configuration configuration_;
};

TEST_F(ConnectServerTest, CheckGameServerLoadUpdate)
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

    size_t hash = ioService_.estabilishConnection();
    ioService_.send(hash, builders::GameServersListRequestBuilder()());

    verifiers::GameServersListResponseVerifier()(ioService_.receive(hash), {{firstServerCode, firstServerLoad, 0},
                                                                            {secondServerCode, secondServerLoad, 0}});

    }
    catch(asioStub::exceptions::NullBufferException&)
    {
        ASSERT_TRUE(false) << "async operation was not queued.";
    }
    catch(asioStub::exceptions::TooBigPayloadException&)
    {
        ASSERT_TRUE(false) << "trying to insert too big payload to tested object.";
    }
    catch(asioStub::exceptions::UnknownSocketException&)
    {
        ASSERT_TRUE(false) << "socket does not exists.";
    }
    catch(asioStub::exceptions::NotCreatedUdpSocketException&)
    {
        ASSERT_TRUE(false) << "udp socket not created.";
    }
    catch(mtEnv::exceptions::EmptyPayloadException&)
    {
        ASSERT_TRUE(false) << "empty payload to verify!";
    }
}

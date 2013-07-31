#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/exceptions.hpp>
#include <connectserver/server.hpp>

namespace asioStub = eMU::mt::env::asioStub;

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

TEST_F(ConnectServerTest, test)
{
    try
    {

    eMU::connectserver::Server server(ioService_, configuration_);
    server.startup();

    size_t hash = ioService_.estabilishConnection();

    eMU::core::network::Payload udpPayload = {0xC1, 0x07, 0xF4, 0x07, 0x00, 0x00, 0x30};
    ioService_.sendTo(udpPayload);

    udpPayload = {0xC1, 0x07, 0xF4, 0x07, 0x14, 0x00, 0x10};
    ioService_.sendTo(udpPayload);

    eMU::core::network::Payload payload = {0xC1, 0x04, 0xF4, 0x06};
    ioService_.send(hash, payload);

    eMU::core::network::Payload payload2 = ioService_.receive(hash);
    ASSERT_TRUE(payload2.size() > 0);

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
}

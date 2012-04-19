#include <gtest/gtest.h>
#include "../../core/tcpConnectionsFactory.hpp"
#include "../../core/tcpConnection.hpp"
#include "tcpSocketMock.hpp"
#include "ioServiceStub.hpp"
#include "types.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;

class connectionsFactoryTest_t: public ::testing::Test {
public:
    connectionsFactoryTest_t():
      factory_(ioService_) {}

    void SetUp() {}

    void TearDown() {}

    eMUNetworkUT::ioServiceStub_t ioService_;
    eMUNetwork::tcp::connectionsFactory_t<eMUNetworkUT::tcp::testConnection_t, eMUNetworkUT::ioServiceStub_t> factory_;
};

TEST_F(connectionsFactoryTest_t, construct) {
    eMUNetworkUT::tcp::testConnection_t *connection = NULL;

    connection = factory_.construct();
    ASSERT_THAT(connection, ::testing::NotNull());
}

TEST_F(connectionsFactoryTest_t, destroy) {
    eMUNetworkUT::tcp::testConnection_t *connection = NULL;

    connection = factory_.construct();
    ASSERT_THAT(connection, ::testing::NotNull());

    connection->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection->socket().expectCall_close();
    factory_.destroy(connection);
}
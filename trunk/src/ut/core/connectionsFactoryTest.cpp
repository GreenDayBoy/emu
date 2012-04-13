#include <gtest/gtest.h>
#include "../../core/connectionsFactory.hpp"
#include "../../core/connection.hpp"
#include "socketMock.hpp"
#include "ioServiceStub.hpp"
#include "connectionObserverMock.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;

class connectionsFactoryTest_t: public ::testing::Test {
public:
    typedef eMUNetwork::connection_t<eMUNetworkUT::socketMock_t,
                                     eMUNetworkUT::ioServiceStub_t> testConnection_t;

    connectionsFactoryTest_t():
      factory_(ioService_) {}

    void SetUp() {}

    void TearDown() {}

    eMUNetworkUT::ioServiceStub_t ioService_;
    eMUNetworkUT::connectionObserverMock_t connectionObserver_;
    eMUNetwork::connectionsFactory_t<testConnection_t, eMUNetworkUT::ioServiceStub_t> factory_;
};

TEST_F(connectionsFactoryTest_t, construct) {
    testConnection_t *connection = NULL;

    connection = factory_.construct(connectionObserver_);
    ASSERT_THAT(connection, ::testing::NotNull());
}

TEST_F(connectionsFactoryTest_t, destroy) {
    testConnection_t *connection = NULL;

    connection = factory_.construct(connectionObserver_);
    ASSERT_THAT(connection, ::testing::NotNull());

    connection->socket().expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    connection->socket().expectCall_close();
    factory_.destroy(connection);
}
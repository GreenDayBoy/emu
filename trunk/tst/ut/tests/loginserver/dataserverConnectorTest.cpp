#include <loginserver/dataserverConnector.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::Return;

using eMU::loginserver::DataserverConnector;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class DataserverConnectorTest: public ::testing::Test
{
protected:
    DataserverConnectorTest():
        connectInfos_({{"127.0.0.1", 55960},
                       {"127.0.0.1", 55962}}),
        connector_(connection_, connectInfos_),
        masterDataserverConnectResult_(false),
        slaveDataserverConnectResult_(false) {}

    void scenario()
    {
        std::vector<bool> connectResults({masterDataserverConnectResult_, slaveDataserverConnectResult_});
        size_t connectResultIndex = 0;

        for(const auto &connectInfo : connectInfos_)
        {
            bool result = connectResults[connectResultIndex];

            boost::asio::ip::tcp::endpoint endpoint1(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(connectInfo.address_), connectInfo.port_));
            EXPECT_CALL(connection_, connect(endpoint1)).WillOnce(Return(result));

            if(result)
            {
                break;
            }
            else
            {
                ++connectResultIndex;
            }
        }

        ASSERT_EQ(masterDataserverConnectResult_ || slaveDataserverConnectResult_, connector_.connect());
    }

    ConnectionMock connection_;
    DataserverConnector::ConnectInfoContainer connectInfos_;
    DataserverConnector connector_;

    bool masterDataserverConnectResult_;
    bool slaveDataserverConnectResult_;
};

TEST_F(DataserverConnectorTest, CheckWhenMasterDataserverIsAlive)
{
    masterDataserverConnectResult_ = true;
    slaveDataserverConnectResult_ = false;

    scenario();
}

TEST_F(DataserverConnectorTest, CheckWhenSlaveDataserverIsAlive)
{
    masterDataserverConnectResult_ = false;
    slaveDataserverConnectResult_ = true;

    scenario();
}

TEST_F(DataserverConnectorTest, CheckWhenNoOneDataserverIsAlive)
{
    masterDataserverConnectResult_ = false;
    slaveDataserverConnectResult_ = false;

    scenario();
}

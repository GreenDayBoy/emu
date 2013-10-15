#include <gtest/gtest.h>
#include <ut/env/connectserver/messageSenderMock.hpp>
#include <connectserver/transactions/gameServersListResponseTransaction.hpp>
#include <interface/gameServersListResponse.hpp>

namespace connectserverEnv = eMU::ut::env::connectserver;
using ::testing::_;
using ::testing::SaveArg;

class GameServersListResponseTransactionTest: public ::testing::Test
{
protected:
    GameServersListResponseTransactionTest():
        hash_(12345) {}

    void compareGameServerInfo(const eMU::connectserver::GameServersList::GameServerInfo &left,
                               const eMU::connectserver::GameServersList::GameServerInfo &right)
    {
        EXPECT_EQ(left.address_, right.address_);
        EXPECT_EQ(left.code_, right.code_);
        EXPECT_EQ(left.load_, right.load_);
        EXPECT_EQ(left.name_, right.name_);
        EXPECT_EQ(left.port_, right.port_);
    }

    void prepareSampleServers()
    {
        eMU::connectserver::GameServersList::GameServerInfo info = {};

        info.address_ = "localhost";
        info.code_ = 20;
        info.load_ = 30;
        info.name_ = "eMU_Test";
        info.port_ = 55901;
        sampleServers_.push_back(info);

        info.address_ = "127.0.0.1";
        info.code_ = 50;
        info.load_ = 1000;
        info.name_ = "eMU_Test2";
        info.port_ = 55902;
        sampleServers_.push_back(info);
    }

    connectserverEnv::MessageSenderMock messageSender_;
    size_t hash_;
    eMU::connectserver::GameServersList::GameServersListContainer sampleServers_;
};

TEST_F(GameServersListResponseTransactionTest, handle)
{
    eMU::connectserver::GameServersList::GameServersListContainer receivedServers;
    EXPECT_CALL(messageSender_, sendGameServersListResponse(hash_, _)).WillOnce(SaveArg<1>(&receivedServers));

    prepareSampleServers();
    eMU::connectserver::transactions::GameServersListResponseTransaction transaction(hash_, sampleServers_, messageSender_);
    transaction.handle();

    ASSERT_EQ(sampleServers_.size(), receivedServers.size());

    for(size_t i = 0; i < receivedServers.size(); ++i)
    {
        compareGameServerInfo(sampleServers_[i], receivedServers[i]);
    }
}

TEST_F(GameServersListResponseTransactionTest, isValidShouldReturnTrueWhenServersListHasCorrectSize)
{
    sampleServers_.resize(eMU::interface::constants::kMaxGameServersListLength);

    eMU::connectserver::transactions::GameServersListResponseTransaction transaction(hash_, sampleServers_, messageSender_);

    ASSERT_TRUE(transaction.isValid());
}

TEST_F(GameServersListResponseTransactionTest, isValidShouldReturnFalseWhenServersListIsToBig)
{
    sampleServers_.resize(eMU::interface::constants::kMaxGameServersListLength + 1);

    eMU::connectserver::transactions::GameServersListResponseTransaction transaction(hash_, sampleServers_, messageSender_);

    ASSERT_FALSE(transaction.isValid());
}

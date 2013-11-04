#include <gtest/gtest.h>
#include <ut/env/connectserver/gameServersListMock.hpp>
#include <connectserver/transactions/gameServerLoadIndicationTransaction.hpp>
#include <interface/gameServerLoadIndication.hpp>

namespace connectserverEnv = eMU::ut::env::connectserver;
using ::testing::Return;
using ::testing::_;

class GameServerLoadIndicationTransactionTest: public ::testing::Test
{
protected:
    GameServerLoadIndicationTransactionTest()
    {
        message_.serverCode_ = 11;
        message_.load_ = 30;
    }

    connectserverEnv::GameServersListMock gameServersList_;

    eMU::interface::GameServerLoadIndication message_;
};

TEST_F(GameServerLoadIndicationTransactionTest, handle)
{
    EXPECT_CALL(gameServersList_, updateGameServerLoad(message_.serverCode_, message_.load_));

    eMU::connectserver::transactions::GameServerLoadIndicationTransaction transaction(message_, gameServersList_);

    transaction.handle();
}

TEST_F(GameServerLoadIndicationTransactionTest, isValidShouldReturnTrueWhenServerCodeExists)
{
    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(true));

    eMU::connectserver::transactions::GameServerLoadIndicationTransaction transaction(message_, gameServersList_);

    ASSERT_TRUE(transaction.isValid());
}

TEST_F(GameServerLoadIndicationTransactionTest, isValidShouldReturnFalseWhenServerCodeDoesNotExist)
{
    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(false));

    eMU::connectserver::transactions::GameServerLoadIndicationTransaction transaction(message_, gameServersList_);

    ASSERT_FALSE(transaction.isValid());
}

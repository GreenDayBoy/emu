#include <gtest/gtest.h>
#include <ut/env/connectserver/gameServersListMock.hpp>
#include <connectserver/transactions/gameServerLoadTransaction.hpp>
#include <interface/gameServerLoadMessage.hpp>

namespace connectserverEnv = eMU::ut::env::connectserver;
using ::testing::Return;
using ::testing::_;

class GameServersLoadTransactionTest: public ::testing::Test
{
protected:
    GameServersLoadTransactionTest()
    {
        message_.serverCode_ = 11;
        message_.load_ = 30;
    }

    connectserverEnv::GameServersListMock gameServersList_;

    eMU::interface::GameServerLoadMessage message_;
};

TEST_F(GameServersLoadTransactionTest, whenServerCodeExistsThenGameServerLoadShouldBeUpdate)
{
    eMU::connectserver::transactions::GameServerLoadTransaction transaction(message_, gameServersList_);

    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(true));
    EXPECT_CALL(gameServersList_, updateGameServerLoad(message_.serverCode_, message_.load_));

    transaction.handle();
}

TEST_F(GameServersLoadTransactionTest, whenServerCodeExistsThenGameServerLoadShouldNotBeUpdated)
{
    eMU::connectserver::transactions::GameServerLoadTransaction transaction(message_, gameServersList_);

    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(false));
    EXPECT_CALL(gameServersList_, updateGameServerLoad(_, _)).Times(0);

    transaction.handle();
}

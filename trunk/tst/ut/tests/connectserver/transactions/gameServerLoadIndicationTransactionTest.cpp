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

TEST_F(GameServerLoadIndicationTransactionTest, whenServerCodeExistsThenShouldBeHandled)
{
    eMU::connectserver::transactions::GameServerLoadIndicationTransaction transaction(message_, gameServersList_);

    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(true));
    EXPECT_CALL(gameServersList_, updateGameServerLoad(message_.serverCode_, message_.load_));

    transaction.handle();
}

TEST_F(GameServerLoadIndicationTransactionTest, whenServerCodeExistsThenShouldBeNotHandled)
{
    eMU::connectserver::transactions::GameServerLoadIndicationTransaction transaction(message_, gameServersList_);

    EXPECT_CALL(gameServersList_, hasGameServer(message_.serverCode_)).WillOnce(Return(false));
    EXPECT_CALL(gameServersList_, updateGameServerLoad(_, _)).Times(0);

    transaction.handle();
}

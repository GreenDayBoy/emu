#include <gtest/gtest.h>
#include <ut/env/connectserver/messageSenderMock.hpp>
#include <ut/env/connectserver/gameServersListMock.hpp>
#include <connectserver/transactions/gameServerAddressRequestTransaction.hpp>

namespace connectserverEnv = eMU::ut::env::connectserver;
using ::testing::Return;
using ::testing::ReturnRef;

class GameServerAddressRequestTransactionTest: public ::testing::Test
{
protected:
    GameServerAddressRequestTransactionTest():
        hash_(12345),
        code_(24) {}

    size_t hash_;
    uint16_t code_;
    connectserverEnv::MessageSenderMock messageSender_;
    connectserverEnv::GameServersListMock gameServersList_;
};

TEST_F(GameServerAddressRequestTransactionTest, whenServerCodeExistsThenShouldBeHandled)
{
    eMU::connectserver::GameServersList::GameServerInfo serverInfo = {code_, "Test", "test.test", 44405, 0};

    EXPECT_CALL(gameServersList_, hasGameServer(code_)).WillOnce(Return(true));
    EXPECT_CALL(gameServersList_, getGameServerInfo(code_)).WillOnce(ReturnRef(serverInfo));
    EXPECT_CALL(messageSender_, sendGameServerAddressResponse(hash_, serverInfo.address_, serverInfo.port_));

    eMU::connectserver::transactions::GameServerAddressRequestTransaction transaction(hash_, messageSender_, gameServersList_, code_);
    transaction.handle();
}

TEST_F(GameServerAddressRequestTransactionTest, whenServerCodeExistsThenShouldBeNotHandled)
{
    EXPECT_CALL(gameServersList_, hasGameServer(code_)).WillOnce(Return(false));

    eMU::connectserver::transactions::GameServerAddressRequestTransaction transaction(hash_, messageSender_, gameServersList_, code_);
    transaction.handle();
}

#pragma once

#include <gmock/gmock.h>
#include <connectserver/gameServersList.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace connectserver
{

class GameServersListMock: public eMU::connectserver::GameServersList
{
public:
    MOCK_METHOD2(updateGameServerLoad, void(uint16_t code, uint32_t load));
    MOCK_CONST_METHOD1(hasGameServer, bool(uint16_t code));
    MOCK_CONST_METHOD1(getGameServerInfo, const GameServerInfo&(uint16_t code));
};

}
}
}
}

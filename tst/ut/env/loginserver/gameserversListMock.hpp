#pragma once

#include <gmock/gmock.h>
#include <loginserver/gameserversList.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace loginserver
{

class GameserversListMock: public eMU::loginserver::GameserversList
{
public:
    MOCK_METHOD2(updateGameserverLoad, void(uint16_t code, uint32_t load));
    MOCK_CONST_METHOD1(hasGameserver, bool(uint16_t code));
    MOCK_CONST_METHOD1(getGameserverInfo, const protocol::loginserver::GameserverInfo&(uint16_t code));
};

}
}
}
}

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
    MOCK_CONST_METHOD0(getServers, const streaming::loginserver::GameserversInfoContainer&());
    MOCK_CONST_METHOD1(hasGameserver, bool(uint16_t code));
    MOCK_CONST_METHOD1(getGameserverInfo, const streaming::loginserver::GameserverInfo&(uint16_t code));
};

}
}
}
}

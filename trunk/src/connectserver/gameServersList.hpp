#pragma once

#include <stdint.h>
#include <string>
#include <core/common/xmlReader.hpp>
#include <common/mockable.hpp>

namespace eMU
{
namespace connectserver
{

class GameServersList
{
public:  
    struct GameServerInfo
    {
        uint16_t code_;
        std::string name_;
        std::string address_;
        uint16_t port_;
        uint32_t load_;
    };

    typedef std::vector<GameServerInfo> GameServersListContainer;

    virtual ~GameServersList();
    void initialize(eMU::core::common::XmlReader &xmlReader);
    const GameServersListContainer& list() const;
    MOCKABLE void updateGameServerLoad(uint16_t code, uint32_t load);
    MOCKABLE bool hasGameServer(uint16_t code) const;
    MOCKABLE const GameServerInfo& getGameServerInfo(uint16_t code) const;

private:
    GameServersListContainer::const_iterator findGameServerInfo(uint16_t code) const;

    GameServersListContainer servers_;
};

}
}

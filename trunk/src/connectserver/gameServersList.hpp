#pragma once

#include <stdint.h>
#include <string>
#include <core/common/xmlReader.hpp>

namespace eMU {
namespace connectserver {

class GameServersList {
public:
    struct GameServerInfo {
        uint16_t code_;
        std::string name_;
        std::string address_;
        uint16_t port_;
        uint32_t load_;
    };

    typedef std::vector<GameServerInfo> GameServersListContainer;

    void initialize(eMU::core::common::XmlReader &xmlReader);
    const GameServersListContainer& list() const;
    void updateGameServerLoad(uint32_t code, uint32_t load);

private:
    GameServersListContainer servers_;
};

}
}

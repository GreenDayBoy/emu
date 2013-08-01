#pragma once

#include <interface/header.hpp>
#include <interface/constants.hpp>

namespace eMU
{
namespace interface
{

struct GameServerInfo
{
    uint16_t code_;
    uint8_t load_;
    uint8_t unknown_;
};

struct GameServersListResponse
{
    LargeMessageHeader header_;
    uint8_t id_;
    uint16_t numberOfServers_;
    GameServerInfo servers_[constants::kMaxGameServersListLength];
};

}
}

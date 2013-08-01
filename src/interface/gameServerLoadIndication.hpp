#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServerLoadIndication
{
    SmallMessageHeader header_;
    uint8_t id_;
    uint16_t serverCode_;
    uint8_t load_;
};

}
}

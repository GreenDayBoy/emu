#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServersListResponse
{
    SmallMessageHeader header_;
    uint16_t serverCode_;
    uint8_t load_;
};

}
}

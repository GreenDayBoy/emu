#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServersListRequest
{
    SmallMessageHeader header_;
    uint8_t id_;
};

}
}

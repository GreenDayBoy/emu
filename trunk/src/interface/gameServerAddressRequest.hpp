#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServerAddressRequest
{
    SmallMessageHeader header_;
    uint8_t id_;
    uint16_t serverCode_;
};

}
}

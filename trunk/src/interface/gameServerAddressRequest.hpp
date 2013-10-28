#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

#pragma pack(push, 1)

struct GameServerAddressRequest
{
    SmallMessageHeader header_;
    uint8_t id_;
    uint16_t serverCode_;
};

#pragma pack(pop)

}
}

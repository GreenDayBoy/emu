#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

#pragma pack(push, 1)

struct GameServerAddressResponse
{
    SmallMessageHeader header_;
    uint8_t id_;
    char address_[16];
    uint16_t port_;
};

#pragma pack(pop)

}
}

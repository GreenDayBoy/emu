#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServerAddressResponse
{
    SmallMessageHeader header_;
    uint8_t subProtocolId_;
    char address_[16];
    uint16_t port_;
};

}
}

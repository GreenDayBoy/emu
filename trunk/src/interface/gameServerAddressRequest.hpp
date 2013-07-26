#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

struct GameServersAddressRequest
{
    SmallMessageHeader header_;
    uint16_t serverCode_;
};

}
}

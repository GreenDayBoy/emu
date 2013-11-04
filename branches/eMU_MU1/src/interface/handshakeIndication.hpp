#pragma once

#include <interface/header.hpp>

namespace eMU
{
namespace interface
{

#pragma pack(push, 1)

struct HandshakeIndication
{
    SmallMessageHeader header_;
    uint8_t id_;
};

#pragma pack(pop)

}
}

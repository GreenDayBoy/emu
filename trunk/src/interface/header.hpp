#pragma once

#include <stdint.h>

namespace eMU
{
namespace interface
{

struct SmallMessageHeader
{
    uint8_t typeId_;
    uint8_t size_;
    uint8_t protocolId_t;
};

struct BigMessageHeader
{
    uint8_t typeId_;
    uint16_t size_;
    uint8_t protocolId_;
};

}
}

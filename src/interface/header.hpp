#pragma once

#include <stdint.h>

namespace eMU
{
namespace interface
{

namespace MessageType
{

enum Type
{
    SMALL_DECRYPTED = 0xC1,
    LARGE_DECRYPTED,
    SMALL_CRYPTED,
    LARGE_CRYPTED,
};

}

struct SmallMessageHeader
{
    uint8_t typeId_;
    uint8_t size_;
    uint8_t protocolId_;
};

struct LargeMessageHeader
{
    uint8_t typeId_;
    uint16_t size_;
    uint8_t protocolId_;
};

}
}

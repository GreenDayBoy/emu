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
}
}

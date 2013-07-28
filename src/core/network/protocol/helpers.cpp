#include <core/network/protocol/helpers.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace protocol
{

uint16_t byteSwap(uint16_t value)
{
    uint8_t highByte = value >> 8;
    uint8_t lowByte = value & 0x00FF;

    return (lowByte << 8) | highByte;
}

}
}
}
}

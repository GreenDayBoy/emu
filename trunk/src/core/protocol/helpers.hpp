#pragma once

#include <stdint.h>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{

uint16_t byteSwap(uint16_t value);
uint8_t getProtocolId(const eMU::core::network::Payload &payload);
size_t getSize(const eMU::core::network::Payload &payload);
bool isCrypted(const eMU::core::network::Payload &payload);
bool hasValidHeader(const eMU::core::network::Payload &payload);

}
}
}

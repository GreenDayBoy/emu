#include <core/protocol/helpers.hpp>
#include <interface/messageTypes.hpp>
#include <interface/header.hpp>
#include <glog/logging.h>

namespace eMU
{
namespace core
{
namespace protocol
{

uint16_t byteSwap(uint16_t value)
{
    uint8_t highByte = value >> 8;
    uint8_t lowByte = value & 0x00FF;

    return (lowByte << 8) | highByte;
}

uint8_t getProtocolId(const eMU::core::network::Payload &payload)
{
    if(payload[0] == interface::MessageType::SMALL_CRYPTED ||
       payload[0] == interface::MessageType::SMALL_DECRYPTED)
    {
        return payload[2];
    }
    else if(payload[0] == interface::MessageType::LARGE_CRYPTED ||
            payload[0] == interface::MessageType::LARGE_DECRYPTED)
    {
        return payload[3];
    }

    return 0;
}

size_t getMessageSize(const eMU::core::network::Payload &payload)
{
    if(payload[0] == interface::MessageType::SMALL_CRYPTED ||
       payload[0] == interface::MessageType::SMALL_DECRYPTED)
    {
        return payload[1];
    }
    else if(payload[0] == interface::MessageType::LARGE_CRYPTED ||
            payload[0] == interface::MessageType::LARGE_DECRYPTED)
    {
        return byteSwap((payload[1] << 8) | payload[2]);
    }

    return 0;
}

bool isCrypted(const eMU::core::network::Payload &payload)
{
    if(payload[0] == interface::MessageType::SMALL_CRYPTED ||
       payload[0] == interface::MessageType::LARGE_CRYPTED)
    {
        return true;
    }

    return false;
}

bool hasValidHeader(const eMU::core::network::Payload &payload)
{
    uint8_t typeId = payload[0];

    if(typeId == interface::MessageType::SMALL_CRYPTED ||
       typeId == interface::MessageType::SMALL_DECRYPTED ||
       typeId == interface::MessageType::LARGE_CRYPTED ||
       typeId == interface::MessageType::LARGE_DECRYPTED)
    {
        return true;
    }

    return false;
}

size_t getHeaderSize(const eMU::core::network::Payload &payload)
{
    if(payload[0] == interface::MessageType::SMALL_DECRYPTED ||
       payload[0] == interface::MessageType::SMALL_CRYPTED)
    {
        return sizeof(interface::SmallMessageHeader);
    }
    else if(payload[0] == interface::MessageType::LARGE_DECRYPTED ||
            payload[0] == interface::MessageType::LARGE_CRYPTED)
    {
        return sizeof(interface::LargeMessageHeader);
    }
    else
    {
        return 0;
    }
}

uint8_t getMessageId(const eMU::core::network::Payload &payload)
{
    size_t messageSize = getMessageSize(payload);
    size_t headerSize = getHeaderSize(payload);

    if(messageSize > headerSize)
    {
        return payload[headerSize];
    }
    else
    {
        return 0;
    }
}

}
}
}

#pragma once

#include <core/network/payload.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

namespace eMU
{
namespace core
{
namespace protocol
{

class PacketsExtractor
{
public:
    typedef std::vector<network::Payload> PacketsContainer;

    class EmptyPayloadException: public common::Exception {};
    class PacketSizeOutOfBoundException: public common::Exception {};
    class NullPacketSizeException: public common::Exception {};

    PacketsExtractor(const network::Payload &payload);

    void extract();
    const PacketsContainer& getPackets() const;

private:
    PacketsExtractor();

    size_t getPacketSize(size_t offset) const;
    size_t calculateDataOffset(size_t offset, size_t packetSize) const;

    const network::Payload &payload_;
    PacketsContainer packets_;
};

}
}
}

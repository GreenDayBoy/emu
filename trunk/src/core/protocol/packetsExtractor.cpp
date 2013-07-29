#include <core/protocol/packetsExtractor.hpp>
#include <core/protocol/helpers.hpp>
#include <core/protocol/exceptions.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{

PacketsExtractor::PacketsExtractor(const network::Payload &payload):
    payload_(payload) {}

void PacketsExtractor::extract()
{
    if(payload_.empty())
    {
        throw exceptions::EmptyPayloadException();
    }

    size_t totalSize = 0;

    do
    {
        network::Payload currentPayload(payload_.begin() + totalSize, payload_.end());
        size_t packetSize = getSize(currentPayload);

        if(currentPayload.size() < packetSize)
        {
            throw exceptions::InvalidPacketSizeException();
        }

        payloads_.push_back(network::Payload(currentPayload.begin(), currentPayload.begin() + packetSize));
        totalSize += packetSize;
    }
    while(totalSize < payload_.size());
}

const PacketsExtractor::PayloadsContainer& PacketsExtractor::payloads() const
{
    return payloads_;
}

}
}
}

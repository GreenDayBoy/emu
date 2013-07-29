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

    while(totalSize < payload_.size())
    {
        network::Payload frame(payload_.begin() + totalSize, payload_.end());

        if(!hasValidHeader(frame))
        {
            throw exceptions::InvalidPacketHeaderException();
        }

        size_t size = getSize(frame);

        if(frame.size() < size)
        {
            throw exceptions::InvalidPacketSizeException();
        }

        payloads_.push_back(network::Payload(frame.begin(), frame.begin() + size));
        totalSize += size;
    }
}

const PacketsExtractor::PayloadsContainer& PacketsExtractor::payloads() const
{
    return payloads_;
}

}
}
}

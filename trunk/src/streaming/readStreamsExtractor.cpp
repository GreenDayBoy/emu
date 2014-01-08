#include <streaming/readStreamsExtractor.hpp>

#include <core/common/logging.hpp>
#include <string.h>

namespace eMU
{
namespace streaming
{

ReadStreamsExtractor::ReadStreamsExtractor(const core::network::Payload &payload):
    payload_(payload) {}

bool ReadStreamsExtractor::extract()
{
    if(payload_.empty())
    {
        eMU_LOG(error) << "Could not extract streams from empty payload!";
        return false;
    }

    size_t currentOffset = 0;

    while(currentOffset < payload_.getSize())
    {
        if(this->isStreamValid(currentOffset))
        {
            size_t size = this->extractStream(currentOffset);

            if(size > 0)
            {
                currentOffset += size;
            }
            else
            {
                eMU_LOG(error) << "Extracted stream has 0 size!";
                return false;
            }
        }
        else
        {
            eMU_LOG(error) << "Invalid stream format!";
            return false;
        }
    }

   return true;
}

ReadStreamsExtractor::StreamsContainer &ReadStreamsExtractor::getStreams()
{
    return streams_;
}

size_t ReadStreamsExtractor::calculateStreamSize(size_t currentOffset) const
{
    return reinterpret_cast<const uint32_t&>(payload_[currentOffset]);
}

size_t ReadStreamsExtractor::calculateStreamOffset(size_t currentOffset) const
{
    return currentOffset + sizeof(uint32_t);
}

bool ReadStreamsExtractor::isStreamValid(size_t currentOffset) const
{
    size_t streamOffset = this->calculateStreamOffset(currentOffset);

    if(streamOffset >= payload_.getSize())
    {
        return false;
    }

    size_t streamSize = this->calculateStreamSize(currentOffset);

    if(streamSize > 0 && streamOffset + streamSize <= payload_.getSize())
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t ReadStreamsExtractor::extractStream(size_t streamOffset)
{
    try
    {
        size_t payloadSize = this->calculateStreamSize(streamOffset) + sizeof(uint32_t);

        core::network::Payload payload;
        payload.setSize(payloadSize);
        memcpy(&payload[0], &payload_[streamOffset], payloadSize);
        streams_.push_back(std::move(ReadStream(payload)));

        return payloadSize;
    }
    catch(const core::network::Payload::SizeOutOfBoundException&)
    {
        eMU_LOG(error) << "Size of payload with stream is out of bound!";
        return 0;
    }
}

}
}

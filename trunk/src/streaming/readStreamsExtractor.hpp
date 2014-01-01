#pragma once

#include <streaming/readStream.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

namespace eMU
{
namespace streaming
{

class ReadStreamsExtractor
{
public:
    typedef std::vector<ReadStream> StreamsContainer;

    ReadStreamsExtractor(const core::network::Payload &payload);

    bool extract();
    StreamsContainer& getStreams();

private:
    ReadStreamsExtractor();

    size_t calculateStreamSize(size_t currentOffset) const;
    size_t calculateStreamOffset(size_t currentOffset) const;
    bool isStreamValid(size_t currentOffset) const;
    size_t extractStream(size_t streamOffset);

    const core::network::Payload &payload_;
    StreamsContainer streams_;
};

}
}

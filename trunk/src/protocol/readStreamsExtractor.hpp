#pragma once

#include <protocol/readStream.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

namespace eMU
{
namespace protocol
{

class ReadStreamsExtractor
{
public:
    typedef std::vector<ReadStream> StreamsContainer;

    class EmptyPayloadException: public core::common::Exception {};
    class EmptyStreamException: public core::common::Exception {};
    class UnknownStreamFormatException: public core::common::Exception {};

    ReadStreamsExtractor(const core::network::Payload &payload);

    void extract();
    StreamsContainer& getStreams();

private:
    ReadStreamsExtractor();

    size_t calculateStreamSize(size_t currentOffset) const;
    size_t calculateStreamOffset(size_t currentOffset) const;
    bool validateStream(size_t currentOffset) const;

    const core::network::Payload &payload_;
    StreamsContainer streams_;
};

}
}

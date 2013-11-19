#pragma once

#include <core/network/payload.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

namespace eMU
{
namespace protocol
{

class ReadStream
{
public:
    class OverflowException: public core::common::Exception {};

    ReadStream(const core::network::Payload &payload);

    uint16_t getId() const;

    template<typename T>
    T readNext()
    {
        size_t offset = currentOffset_;
        currentOffset_ += sizeof(T);

        return readFromOffset<T>(offset);
    }

    std::string readNextString(size_t length);

private:
    ReadStream();

    template<typename T>
    T readFromOffset(size_t offset) const
    {
        if(sizeof(T) + offset > payload_.getSize())
        {
            throw OverflowException();
        }

        return *(reinterpret_cast<const T*>(&payload_[offset]));
    }

    std::string readStringFromOffset(size_t offset, size_t length) const;

    core::network::Payload payload_;
    size_t currentOffset_;
};

}
}

#include <protocol/readStream.hpp>

#include <string.h>

namespace eMU
{
namespace protocol
{

ReadStream::ReadStream(const core::network::Payload &payload):
    payload_(payload),
    currentOffset_(6)
{
}

uint16_t ReadStream::getId() const
{
    return this->readFromOffset<uint16_t>(4);
}

std::string ReadStream::readStringFromOffset(size_t offset, size_t length) const
{
    std::string value;

    for(size_t i = 0; i < length; ++i)
    {
        value.push_back(this->readFromOffset<std::string::value_type>(offset + i));
    }

    return std::move(value);
}

std::string ReadStream::readNextString(size_t length)
{
    size_t offset = currentOffset_;
    currentOffset_ += length;

    return std::move(this->readStringFromOffset(offset, length));
}

}
}

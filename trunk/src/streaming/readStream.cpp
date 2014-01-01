#include <streaming/readStream.hpp>

#include <string.h>

namespace eMU
{
namespace streaming
{

ReadStream::ReadStream(const core::network::Payload &payload):
    payload_(payload),
    currentOffset_(6)
{
}

ReadStream::ReadStream():
    currentOffset_(0) {}

uint16_t ReadStream::getId() const
{
    return this->readFromOffset<uint16_t>(4);
}

size_t ReadStream::getSize() const
{
    return payload_.getSize();
}

std::string ReadStream::readNextString(size_t length)
{
    std::string value;

    for(size_t i = 0; i < length; ++i)
    {
        value.push_back(this->readNext<std::string::value_type>());
    }

    return std::move(value);
}

std::wstring ReadStream::readNextWideString(size_t length)
{
    std::wstring value;

    for(size_t i = 0; i < length; ++i)
    {
        value.push_back(this->readNext<int16_t>());
    }

    return std::move(value);
}

const core::network::Payload& ReadStream::getPayload() const
{
    return payload_;
}

}
}

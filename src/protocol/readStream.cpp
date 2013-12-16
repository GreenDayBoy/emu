#include <protocol/readStream.hpp>

#include <string.h>
#include <iomanip>

namespace eMU
{
namespace protocol
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

std::ostream& operator<<(std::ostream &out, const ReadStream &stream)
{
    for(size_t i = 0; i < stream.payload_.getSize(); ++i)
    {
        out << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(stream.payload_[i]) << " ";
    }

    return out;
}

}
}

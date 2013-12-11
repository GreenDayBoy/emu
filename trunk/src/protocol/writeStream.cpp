#include <protocol/writeStream.hpp>

#include <string.h>

namespace eMU
{
namespace protocol
{

WriteStream::WriteStream(uint16_t id):
    currentOffset_(sizeof(uint32_t)),
    size_(reinterpret_cast<uint32_t&>(payload_[0]))
{
    this->writeNext<uint16_t>(id);
}

WriteStream::WriteStream():
    WriteStream(0) {}

const core::network::Payload& WriteStream::getPayload() const
{
    return payload_;
}

template<>
void WriteStream::writeNext(const std::string &value)
{
    for(size_t i = 0; i < value.length(); ++i)
    {
        this->writeNext<std::string::value_type>(value[i]);
    }
}

template<>
void WriteStream::writeNext(const std::wstring &value)
{
    for(size_t i = 0; i < value.length(); ++i)
    {
        this->writeNext<char16_t>(value[i]);
    }
}

}
}

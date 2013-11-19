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
    this->write<uint16_t>(id);
}

const core::network::Payload& WriteStream::getPayload() const
{
    return payload_;
}

template<>
void WriteStream::write(const std::string &value)
{
    for(size_t i = 0; i < value.length(); ++i)
    {
        this->write<std::string::value_type>(value[i]);
    }
}

size_t WriteStream::getSize() const
{
    return size_;
}

}
}

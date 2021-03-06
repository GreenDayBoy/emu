#include <core/network/payload.hpp>

#include <iomanip>

namespace eMU
{
namespace core
{
namespace network
{

Payload::Payload():
    data_(getMaxSize(), 0),
    size_(0) {}

size_t Payload::getMaxSize()
{
    return 4096;
}

size_t Payload::getSize() const
{
    return size_;
}

void Payload::setSize(size_t newSize)
{
    if(newSize > getMaxSize())
    {
        throw SizeOutOfBoundException();
    }

    size_ = newSize;
}

void Payload::clear()
{
    data_.clear();
    data_.resize(getMaxSize(), 0);
    size_ = 0;
}

bool Payload::empty() const
{
    return size_ == 0;
}

uint8_t& Payload::operator[](size_t offset)
{
    return data_[offset];
}

const uint8_t& Payload::operator[](size_t offset) const
{
    return data_[offset];
}

std::ostream& operator<<(std::ostream &out, const Payload &payload)
{
    for(size_t i = 0; i < payload.getSize(); ++i)
    {
        out << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(payload[i]) << " ";
    }

    return out;
}

}
}
}

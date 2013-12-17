#pragma once

#include <core/network/payload.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

namespace eMU
{
namespace protocol
{

class WriteStream
{
public:
    class OverflowException: public core::common::Exception {};

    WriteStream(uint16_t id);
    WriteStream();

    const core::network::Payload& getPayload() const;

    template<typename T>
    void writeNext(const T &value)
    {
        size_t typeSize = sizeof(T);

        if(typeSize + currentOffset_ > payload_.getMaxSize())
        {
            throw OverflowException();
        }

        T &destination = reinterpret_cast<T&>(payload_[currentOffset_]);
        destination = value;

        currentOffset_ += typeSize;
        size_ += typeSize;
        payload_.setSize(size_ + sizeof(uint32_t));
    }

    void writeNextWideString(const std::wstring &value);
    void writeNextString(const std::string &value);

    friend std::ostream& operator<<(std::ostream &out, const WriteStream &stream);

private:
    core::network::Payload payload_;
    uint32_t currentOffset_;
    uint32_t &size_;
};

}
}

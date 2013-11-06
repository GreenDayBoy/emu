#pragma once

#include <core/common/exception.hpp>

#include <vector>
#include <stdint.h>
#include <cstdlib>

namespace eMU
{
namespace core
{
namespace network
{

class Payload
{
public:                                                       
    typedef std::vector<uint8_t> Data;

    class SetOverflowException: public common::Exception {};
    class GetOverflowException: public common::Exception {};
    class SizeOutOfBoundException: public common::Exception {};

    Payload();

    static size_t getMaxSize();
    size_t getSize() const;
    void setSize(size_t newSize);
    void clear();

    uint8_t& operator[](size_t offset);
    const uint8_t& operator[](size_t offset) const;

    template<typename T>
    T getValue(size_t offset) const
    {
        if(sizeof(T) + offset > getMaxSize())
        {
            throw GetOverflowException();
        }

        return *(reinterpret_cast<const T*>(&data_[offset]));
    }

    template<typename T>
    void setValue(size_t offset, const T &value)
    {
        size_t typeSize = sizeof(T);

        if(typeSize + offset > getMaxSize())
        {
            throw SetOverflowException();
        }

        T &destination = reinterpret_cast<T&>(data_[offset]);
        destination = value;

        size_ += typeSize;
    }

private:
    Data data_;
    size_t size_;
};

}
}
}

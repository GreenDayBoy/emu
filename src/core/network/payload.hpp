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

    class InsertOverflowException: public common::Exception {};
    class GetOverflowException: public common::Exception {};
    class SizeOutOfBoundException: public common::Exception {};

    Payload();

    static size_t getMaxSize();
    size_t getSize() const;
    void setSize(size_t newSize);
    void clear();
    bool empty() const;

    uint8_t& operator[](size_t offset);
    const uint8_t& operator[](size_t offset) const;

    template<typename T>
    T getValue(size_t offset) const
    {
        if(sizeof(T) + offset > getSize())
        {
            throw GetOverflowException();
        }

        return *(reinterpret_cast<const T*>(&data_[offset]));
    }

    template<typename T>
    void insert(const T &value)
    {
        size_t typeSize = sizeof(T);

        if(typeSize + size_ > getMaxSize())
        {
            throw InsertOverflowException();
        }

        T &destination = reinterpret_cast<T&>(data_[size_]);
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

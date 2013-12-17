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

    class SizeOutOfBoundException: public common::Exception {};

    Payload();

    static size_t getMaxSize();
    size_t getSize() const;
    void setSize(size_t newSize);
    void clear();
    bool empty() const;

    uint8_t& operator[](size_t offset);
    const uint8_t& operator[](size_t offset) const;

private:
    Data data_;
    size_t size_;
};

}
}
}

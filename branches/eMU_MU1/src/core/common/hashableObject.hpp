#pragma once

#include <cstddef>

namespace eMU
{
namespace core
{
namespace common
{

class HashableObject
{
public:
    virtual ~HashableObject();

    virtual size_t hash() const;
};

}
}
}

#pragma once

#include <stdint.h>
#include <cstring>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace messages
{
namespace builders
{

class GameServersListRequestBuilder
{
public:
    eMU::core::network::Payload operator()();
};

}
}
}
}
}

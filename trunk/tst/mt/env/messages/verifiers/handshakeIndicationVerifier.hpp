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
namespace verifiers
{

class HandshakeIndicationVerifier
{
public:
    void operator()(const core::network::Payload &payload);
};

}
}
}
}
}

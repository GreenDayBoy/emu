#pragma once

#include <stdint.h>
#include <string>
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

class GameServerAddressResponseVerifier
{
public:
    void operator()(const core::network::Payload &payload, std::string address, uint16_t port);
};

}
}
}
}
}

#pragma once

#include <stdint.h>
#include <cstring>
#include <core/network/buffer.hpp>
#include <interface/gameServersListResponse.hpp>

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

class GameServersListResponseVerifier
{
public:
    void operator()(const core::network::Payload &payload, const std::vector<interface::GameServerInfo> &servers);
};

}
}
}
}
}

#pragma once

#include <gmock/gmock.h>
#include <core/network/payload.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace core
{
namespace network
{

class SamplePayloads
{
public:
    SamplePayloads();

    eMU::core::network::Payload payload1_;
    eMU::core::network::Payload payload2_;
    eMU::core::network::Payload payload3_;

    eMU::core::network::Payload fullFilledPayload_;
    eMU::core::network::Payload halfFilledPayload_;

private:
    void preparePayload(eMU::core::network::Payload &payload, size_t bytes);
};

}
}
}
}
}

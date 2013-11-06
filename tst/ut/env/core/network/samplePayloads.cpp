#include <ut/env/core/network/samplePayloads.hpp>

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

SamplePayloads::SamplePayloads()
{
    preparePayload(fullFilledPayload_, eMU::core::network::Payload::getMaxSize());
    preparePayload(halfFilledPayload_, eMU::core::network::Payload::getMaxSize() / 2);
    preparePayload(payload1_, 30);
    preparePayload(payload2_, 60);
    preparePayload(payload3_, 90);
}

void SamplePayloads::preparePayload(eMU::core::network::Payload &payload, size_t bytes)
{
    for(size_t i = 0; i < bytes; ++i)
    {
        payload.setValue<uint8_t>(i, static_cast<uint8_t>(i));
    }
}

}
}
}
}
}

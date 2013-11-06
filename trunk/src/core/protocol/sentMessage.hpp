#pragma once

#include <core/network/payload.hpp>

#include <stdint.h>

namespace eMU
{
namespace core
{
namespace protocol
{

class SentMessage
{
public:
    virtual ~SentMessage();

    const network::Payload& getPayload() const;

protected:
    network::Payload payload_;
};

}
}
}

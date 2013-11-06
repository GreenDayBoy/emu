#pragma once

#include <core/network/payload.hpp>

#include <stdint.h>

namespace eMU
{
namespace core
{
namespace protocol
{

class ReceivedMessage
{
public:
    ReceivedMessage(const network::Payload &payload);
    virtual ~ReceivedMessage();

private:
    ReceivedMessage();

protected:
    const network::Payload &payload_;
};

}
}
}

#include <core/protocol/sentMessage.hpp>

#include <string.h>

namespace eMU
{
namespace core
{
namespace protocol
{

SentMessage::~SentMessage() {}

const network::Payload& SentMessage::getPayload() const
{
    return payload_;
}

}
}
}

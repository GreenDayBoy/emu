#include <core/protocol/receivedMessage.hpp>

#include <string.h>

namespace eMU
{
namespace core
{
namespace protocol
{

ReceivedMessage::ReceivedMessage(const network::Payload &payload):
    payload_(payload) {}

ReceivedMessage::~ReceivedMessage() {}

}
}
}

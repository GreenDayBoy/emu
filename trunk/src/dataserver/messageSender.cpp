#include <dataserver/messageSender.hpp>

namespace eMU
{
namespace dataserver
{

MessageSender::MessageSender(const SendFunctor &sendFunctor):
    sendFunctor_(sendFunctor) {}

MessageSender::~MessageSender() {}

void MessageSender::sendCheckAccountResponse(size_t hash, uint32_t checkResult)
{
    core::network::Payload packet;

    uint32_t messageSize = sizeof(hash) + sizeof(checkResult);

    packet.insert(messageSize);
    packet.insert(hash);
    packet.insert(checkResult);
}

}
}

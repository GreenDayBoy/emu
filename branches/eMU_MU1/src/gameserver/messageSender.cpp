#include <gameserver/messageSender.hpp>

namespace eMU
{
namespace gameserver
{

MessageSender::MessageSender(const SendFunctor &sendFunctor):
    sendFunctor_(sendFunctor) {}
MessageSender::~MessageSender() {}

}
}

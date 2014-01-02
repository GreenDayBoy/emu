#include <loginserver/gameserverProtocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

GameserverProtocol::GameserverProtocol(Context &context):
    protocols::Udp(context),
    context_(context) {}

void GameserverProtocol::handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
}

}
}

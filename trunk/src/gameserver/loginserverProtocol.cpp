#include <gameserver/loginserverProtocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

LoginserverProtocol::LoginserverProtocol(Context &context):
    protocols::Udp(context),
    context_(context) {}

void LoginserverProtocol::handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
}

}
}

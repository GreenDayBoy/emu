#include <loginserver/udpProtocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

UdpProtocol::UdpProtocol(Context &context):
    protocols::Udp(context),
    context_(context) {}

void UdpProtocol::handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
}

}
}

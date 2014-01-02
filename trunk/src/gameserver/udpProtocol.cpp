#include <gameserver/udpProtocol.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

UdpProtocol::UdpProtocol(Context &context):
    protocols::Udp(context),
    context_(context) {}

void UdpProtocol::handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
    uint16_t streamId = stream.getId();

    if(streamId == streaming::gameserver::streamIds::kRegisterUserRequest)
    {

    }
}

}
}

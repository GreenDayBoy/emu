#include <loginserver/udpProtocol.hpp>
#include <loginserver/transactions/registerUserResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace loginserver
{

UdpProtocol::UdpProtocol(Context &context):
    protocols::Udp(context),
    context_(context) {}

void UdpProtocol::handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
    uint16_t streamId = stream.getId();

    if(streamId == streaming::gameserver::streamIds::kRegisterUserResponse)
    {
        streaming::gameserver::RegisterUserResponse response(stream);
        transactions::RegisterUserResponse(context_.getUsersFactory(), context_.getGameserversList(), response).handle();
    }
}

}
}

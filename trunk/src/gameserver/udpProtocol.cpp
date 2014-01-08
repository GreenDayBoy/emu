#include <gameserver/udpProtocol.hpp>
#include <gameserver/transactions/registerUserRequest.hpp>

#include <streaming/gameserver/streamIds.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>

#include <core/common/logging.hpp>

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
        streaming::gameserver::RegisterUserRequest request(stream);
        transactions::RegisterUserRequest(senderEndpoint,
                                          context_.getUdpConnection(),
                                          context_.getUserRegistrationInfos(),
                                          context_.getGameserverCode(),
                                          request).handle();
    }
}

}
}

#include <protocols/udp.hpp>
#include <streaming/readStreamsExtractor.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace protocols
{

Udp::Udp(contexts::Udp &context):
    context_(context) {}

void Udp::attach(core::network::udp::Connection::Pointer connection)
{
    context_.setUdpConnection(connection);
}

void Udp::dispatch(core::network::udp::Connection::Pointer connection, const boost::asio::ip::udp::endpoint &senderEndpoint)
{
    streaming::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
    if(!readStreamsExtractor.extract())
    {
        eMU_LOG(error) << "Streams extraction failed.";
        return;
    }

    for(const auto &stream : readStreamsExtractor.getStreams())
    {
        eMU_LOG(info) << "Udp protocol, received stream id: " << stream.getId();
        this->handleReadStream(stream, senderEndpoint);
    }
}

void Udp::detach(core::network::udp::Connection::Pointer)
{
    context_.setUdpConnection(nullptr);
}

}
}

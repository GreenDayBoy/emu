#include <protocols/udp.hpp>
#include <streaming/readStreamsExtractor.hpp>

#include <glog/logging.h>

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
        LOG(ERROR) << "Streams extraction failed.";
        return;
    }

    for(const auto &stream : readStreamsExtractor.getStreams())
    {
        LOG(INFO) << "Udp protocol, received stream, id: " << stream.getId();
        this->handleReadStream(stream, senderEndpoint);
    }
}

void Udp::detach(core::network::udp::Connection::Pointer)
{
    context_.setUdpConnection(nullptr);
}

}
}

#include <protocols/client.hpp>
#include <streaming/readStreamsExtractor.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace protocols
{

Client::Client(contexts::Client &context):
    context_(context) {}

bool Client::attach(core::network::tcp::Connection::Pointer connection)
{
    context_.setClientConnection(connection);

    return true;
}

void Client::detach(core::network::tcp::Connection::Pointer connection)
{
    LOG(INFO) << "Client connection closed.";

    context_.setClientConnection(nullptr);
}

bool Client::dispatch(core::network::tcp::Connection::Pointer connection)
{
    streaming::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
    if(!readStreamsExtractor.extract())
    {
        LOG(ERROR) << "Streams extraction failed.";
        return false;
    }

    for(const auto &stream : readStreamsExtractor.getStreams())
    {
        LOG(INFO) << "Client protocol, received stream, id: " << stream.getId();

        if(!this->handleReadStream(stream))
        {
            return false;
        }
    }

    return true;
}

}
}

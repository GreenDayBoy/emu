#include <protocols/client.hpp>
#include <streaming/readStreamsExtractor.hpp>

#include <core/common/logging.hpp>

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

void Client::detach(core::network::tcp::Connection::Pointer)
{
    eMU_LOG(info) << "Client connection closed.";

    context_.setClientConnection(nullptr);
}

bool Client::dispatch(core::network::tcp::Connection::Pointer connection)
{
    streaming::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
    if(!readStreamsExtractor.extract())
    {
        eMU_LOG(error) << "Streams extraction failed.";
        return false;
    }

    for(const auto &stream : readStreamsExtractor.getStreams())
    {
        eMU_LOG(info) << "Client protocol, received stream, id: " << stream.getId();

        if(!this->handleReadStream(stream))
        {
            return false;
        }
    }

    return true;
}

}
}

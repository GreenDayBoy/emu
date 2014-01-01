#include <gameserver/dataserverProtocol.hpp>
#include <protocol/readStreamsExtractor.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

DataserverProtocol::DataserverProtocol(Context &context):
    context_(context) {}

bool DataserverProtocol::attach(core::network::tcp::Connection::Pointer connection)
{
    context_.setDataserverConnection(connection);

    return true;
}

void DataserverProtocol::detach(core::network::tcp::Connection::Pointer connection)
{
    LOG(INFO) << "Dataserver connection closed.";

    // TODO: Recovery when connection to dataserver lost.
    context_.setDataserverConnection(nullptr);
}

bool DataserverProtocol::dispatch(core::network::tcp::Connection::Pointer connection)
{
    protocol::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
    if(!readStreamsExtractor.extract())
    {
        LOG(ERROR) << "Streams extraction failed.";
        return false;
    }

    for(const auto &stream : readStreamsExtractor.getStreams())
    {
        this->handleReadStream(stream);
    }

    context_.getTransactionsManager().dequeueAll();

    return true;
}

void DataserverProtocol::handleReadStream(const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    LOG(INFO) << "Dataserver, received stream, id: " << messageId;
}

}
}

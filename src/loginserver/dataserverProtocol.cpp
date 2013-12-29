#include <loginserver/dataserverProtocol.hpp>
#include <loginserver/transactions/checkAccountResponse.hpp>
#include <loginserver/transactions/faultIndication.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/dataserver/messageIds.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
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

    if(messageId == protocol::dataserver::MessageIds::kCheckAccountResponse)
    {
        protocol::dataserver::CheckAccountResponse response(stream);
        context_.getTransactionsManager().queue(new transactions::CheckAccountResponse(context_.getUsersFactory(), response));
    }
    if(messageId == protocol::dataserver::MessageIds::kFaultIndication)
    {
        protocol::dataserver::FaultIndication indication(stream);
        context_.getTransactionsManager().queue(new transactions::FaultIndication(context_.getUsersFactory(), indication));
    }
}

}
}

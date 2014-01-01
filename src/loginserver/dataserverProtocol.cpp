#include <loginserver/dataserverProtocol.hpp>
#include <loginserver/transactions/checkAccountResponse.hpp>
#include <loginserver/transactions/faultIndication.hpp>
#include <streaming/dataserver/streamIds.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

DataserverProtocol::DataserverProtocol(Context &context):
    protocols::Client(context),
    context_(context) {}

bool DataserverProtocol::handleReadStream(const streaming::ReadStream &stream)
{
    uint16_t streamId = stream.getId();

    if(streamId == streaming::dataserver::streamIds::kCheckAccountResponse)
    {
        streaming::dataserver::CheckAccountResponse response(stream);
        context_.getTransactionsManager().queue(new transactions::CheckAccountResponse(context_.getUsersFactory(), response));
    }
    if(streamId == streaming::dataserver::streamIds::kFaultIndication)
    {
        streaming::dataserver::FaultIndication indication(stream);
        context_.getTransactionsManager().queue(new transactions::FaultIndication(context_.getUsersFactory(), indication));
    }

    context_.getTransactionsManager().dequeueAll();

    return true;
}

}
}

#include <dataserver/protocol.hpp>
#include <dataserver/transactions/checkAccountRequest.hpp>

#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace dataserver
{

Protocol::Protocol(Context &context):
    protocols::Server<User>(context),
    context_(context) {}

bool Protocol::handleReadStream(User &user, const streaming::ReadStream &stream)
{
    uint16_t streamId = stream.getId();
    bool result = false;

    if(streamId == streaming::dataserver::streamIds::kCheckAccountRequest)
    {
        streaming::dataserver::CheckAccountRequest request(stream);
        context_.getTransactionsManager().queue(new transactions::CheckAccountRequest(user, context_.getSqlInterface(), request));

        result = true;
    }

    if(result)
    {
        context_.getTransactionsManager().dequeueAll();
    }

    return result;
}


}
}

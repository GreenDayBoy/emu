#include <loginserver/protocol.hpp>
#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <loginserver/transactions/gameserversListRequest.hpp>
#include <loginserver/transactions/loginRequest.hpp>

#include <streaming/loginserver/streamIds.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

Protocol::Protocol(Context &context):
    protocols::Server<User>(context),
    context_(context) {}

bool Protocol::handleReadStream(User &user, const streaming::ReadStream &stream)
{
    uint16_t streamId = stream.getId();
    bool result = false;

    if(streamId == streaming::loginserver::streamIds::kLoginRequest)
    {
        streaming::loginserver::LoginRequest request(stream);
        context_.getTransactionsManager().queue(new transactions::LoginRequest(user, context_.getClientConnection(), request));

        result = true;
    }
    else if(streamId == streaming::loginserver::streamIds::kGameserversListRequest)
    {
        streaming::loginserver::GameserversListRequest request(stream);
        context_.getTransactionsManager().queue((new transactions::GameserversListRequest(user, context_.getGameserversList(), request)));

        result = true;
    }
    else if(streamId == streaming::loginserver::streamIds::kGameserverDetailsRequest)
    {
        streaming::loginserver::GameserverDetailsRequest request(stream);
        context_.getTransactionsManager().queue((new transactions::GameserverDetailsRequest(user, context_.getGameserversList(), request)));

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

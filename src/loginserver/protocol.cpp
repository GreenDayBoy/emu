#include <loginserver/protocol.hpp>
#include <loginserver/transactions/gameserverDetailsRequest.hpp>
#include <loginserver/transactions/gameserversListRequest.hpp>
#include <loginserver/transactions/loginRequest.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

Protocol::Protocol(Context &context):
    context_(context) {}

bool Protocol::attach(core::network::tcp::Connection::Pointer connection)
{
    if(context_.getUsersFactory().getObjects().size() > context_.getMaxNumberOfUsers())
    {
        LOG(WARNING) << "Max number of users reached.";
        return false;
    }

    User &user = context_.getUsersFactory().create(connection);

    LOG(INFO) << "hash: " << user.getHash() <<  ", user registered.";

    return true;
}

void Protocol::detach(core::network::tcp::Connection::Pointer connection)
{
    try
    {
        User &user = context_.getUsersFactory().find(connection);

        LOG(INFO) << "hash: " << user.getHash() <<  ", user closed.";

        context_.getUsersFactory().destroy(user);
        connection->close();
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "user not found!";
        connection->close();
    }
}

bool Protocol::dispatch(core::network::tcp::Connection::Pointer connection)
{
    protocol::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
    if(!readStreamsExtractor.extract())
    {
        LOG(ERROR) << "Streams extraction failed.";
        return false;
    }

    try
    {
        User &user = context_.getUsersFactory().find(connection);

        for(const auto &stream : readStreamsExtractor.getStreams())
        {
            if(!this->handleReadStream(user, stream))
            {
                return false;
            }
        }

        context_.getTransactionsManager().dequeueAll();
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "user not found!";
        return false;
    }

    return true;
}

bool Protocol::handleReadStream(User &user, const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    LOG(INFO) << "hash: " << user.getHash() << ", received stream, id: " << messageId;

    if(messageId == protocol::loginserver::MessageIds::kLoginRequest)
    {
        protocol::loginserver::LoginRequest request(stream);
        context_.getTransactionsManager().queue(new transactions::LoginRequest(user, context_.getDataserverConnection(), request));

        return true;
    }
    else if(messageId == protocol::loginserver::MessageIds::kGameserversListRequest)
    {
        protocol::loginserver::GameserversListRequest request(stream);
        context_.getTransactionsManager().queue((new transactions::GameserversListRequest(user, context_.getGameserversList(), request)));

        return true;
    }
    else if(messageId == protocol::loginserver::MessageIds::kGameserverDetailsRequest)
    {
        protocol::loginserver::GameserverDetailsRequest request(stream);
        context_.getTransactionsManager().queue((new transactions::GameserverDetailsRequest(user, context_.getGameserversList(), request)));

        return true;
    }

    return false;
}


}
}

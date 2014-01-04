#include <gameserver/protocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

Protocol::Protocol(Context &context):
    protocols::Server<User>(context),
    context_(context) {}

bool Protocol::attach(core::network::tcp::Connection::Pointer connection)
{
    if(context_.getUserRegistrationInfos().empty())
    {
        LOG(ERROR) << "Connection was not previously registered.";
        return false;
    }

    if(protocols::Server<User>::attach(connection))
    {
        User &user = *context_.getUsersFactory().getObjects().back();

        streaming::gameserver::UserRegistrationInfo &registrationInfo = context_.getUserRegistrationInfos().front();
        LOG(INFO) << "Registration info, hash: " << registrationInfo.userHash_ << ", accountId: " << registrationInfo.accountId_;

        user.setAccountId(registrationInfo.accountId_);
        context_.getUserRegistrationInfos().pop_front();
        return true;
    }

    return false;
}

bool Protocol::handleReadStream(User &user, const streaming::ReadStream &stream)
{
    return true;
}

}
}

#include <gameserver/protocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

Protocol::Protocol(Context &context):
    protocols::Server<User>(context),
    context_(context) {}

bool Protocol::handleReadStream(User &user, const streaming::ReadStream &stream)
{
    return true;
}


}
}

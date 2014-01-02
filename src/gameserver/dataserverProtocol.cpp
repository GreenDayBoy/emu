#include <gameserver/dataserverProtocol.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace gameserver
{

DataserverProtocol::DataserverProtocol(Context &context):
    protocols::Client(context),
    context_(context) {}

bool DataserverProtocol::handleReadStream(const streaming::ReadStream &stream)
{
    return true;
}

}
}

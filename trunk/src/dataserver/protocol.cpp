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

    if(streamId == streaming::dataserver::streamIds::kCheckAccountRequest)
    {
        streaming::dataserver::CheckAccountRequest request(stream);
        transactions::CheckAccountRequest(user, context_.getSqlInterface(), request).handle();

        return true;
    }

    return false;
}


}
}

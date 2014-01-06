#include <gameserver/dataserverProtocol.hpp>
#include <gameserver/transactions/charactersListResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>

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
    uint16_t streamId = stream.getId();

    if(streamId == streaming::dataserver::streamIds::kCharactersListResponse)
    {
        streaming::dataserver::CharactersListResponse response(stream);
        transactions::CharactersListResponse(context_.getUsersFactory(), response).handle();
    }

    return true;
}

}
}

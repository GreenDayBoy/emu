#include <gameserver/dataserverProtocol.hpp>
#include <gameserver/transactions/charactersListResponse.hpp>
#include <gameserver/transactions/characterCreateResponse.hpp>
#include <gameserver/transactions/faultIndication.hpp>
#include <streaming/dataserver/streamIds.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/dataserver/faultIndication.hpp>

#include <core/common/logging.hpp>

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
    if(streamId == streaming::dataserver::streamIds::kCharacterCreateResponse)
    {
        streaming::dataserver::CharacterCreateResponse response(stream);
        transactions::CharacterCreateResponse(context_.getUsersFactory(), response).handle();
    }
    if(streamId == streaming::dataserver::streamIds::kFaultIndication)
    {
        streaming::dataserver::FaultIndication indication(stream);
        transactions::FaultIndication(context_.getUsersFactory(), indication).handle();
    }

    return true;
}

}
}

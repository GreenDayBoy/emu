#include <dataserver/transactions/characterCreateRequest.hpp>
#include <streaming/dataserver/characterCreateResponse.hpp>

#include <sstream>
#include <core/common/logging.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CharacterCreateRequest::CharacterCreateRequest(User &user,
                                               database::SqlInterface &sqlInterface,
                                               const streaming::dataserver::CharacterCreateRequest &request):
    DatabaseTransaction(user, sqlInterface, request.getUserHash()),
    request_(request) {}

void CharacterCreateRequest::handleValid()
{
    eMU_LOG(info) << "hash: " << user_.getHash()
        << ", userHash: " << request_.getUserHash()
        << ", accountId: " << request_.getAccountId()
        << ", character name: " << request_.getCharacterCreateInfo().name_;

    std::stringstream query;
    query << "SELECT eMU_CharacterCreate('" << request_.getAccountId() << "'"
        << ", '" << request_.getCharacterCreateInfo().name_ << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().skin_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().race_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().face_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().faceScars_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().hairType_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().hairColor_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().tatoo_) << "'"
        << ", '" << static_cast<uint32_t>(request_.getCharacterCreateInfo().skinColor_) << "');";

    if(!sqlInterface_.executeQuery(query.str()))
    {
        this->sendFaultIndication(sqlInterface_.getErrorMessage());
        return;
    }

    const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();

    if(queryResult.getRows().empty())
    {
        eMU_LOG(error) << "Empty query results for accountId: " << request_.getAccountId();
        this->sendFaultIndication("Character create query result is empty");

        return;
    }

    streaming::dataserver::CharacterCreateResult result = static_cast<streaming::dataserver::CharacterCreateResult>(queryResult.getRows()[0].getValue<uint32_t>(0));
    streaming::dataserver::CharacterCreateResponse response(request_.getUserHash(), result);
    user_.getConnection().send(response.getWriteStream().getPayload());
}

}
}
}

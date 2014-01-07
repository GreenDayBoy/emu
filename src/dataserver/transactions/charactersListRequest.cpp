#include <dataserver/transactions/charactersListRequest.hpp>
#include <streaming/dataserver/charactersListResponse.hpp>

#include <sstream>
#include <glog/logging.h>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CharactersListRequest::CharactersListRequest(User &user,
                                             database::SqlInterface &sqlInterface,
                                             const streaming::dataserver::CharactersListRequest &request):
    DatabaseTransaction(user, sqlInterface, request.getUserHash()),
    request_(request) {}

void CharactersListRequest::handleValid()
{
    LOG(INFO) << "hash: " << user_.getHash()
              << ", userHash: " << request_.getUserHash()
              << ", accountId: " << request_.getAccountId();

    std::stringstream query;
    query << "SELECT hairColor, hairType, level, name, race, tutorialState FROM characters WHERE accountId='" << request_.getAccountId() << "';";

    if(!sqlInterface_.executeQuery(query.str()))
    {
        this->sendFaultIndication(sqlInterface_.getErrorMessage());
        return;
    }

    const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();
    streaming::common::CharacterInfoContainer characters;

    for(const auto &row : queryResult.getRows())
    {
        streaming::common::CharacterInfo character;
        character.hairColor_ = row.getValue<uint32_t>("hairColor");
        character.hairType_ = row.getValue<uint32_t>("hairType");
        character.level_ = row.getValue<uint32_t>("level");
        character.name_ = row.getValue<std::string>("name");
        character.race_ = row.getValue<uint32_t>("race");
        character.tutorialState_ = row.getValue<uint16_t>("tutorialState");

        characters.push_back(character);
    }

    streaming::dataserver::CharactersListResponse response(request_.getUserHash(), characters);
    user_.getConnection().send(response.getWriteStream().getPayload());
}

}
}
}

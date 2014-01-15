#include <dataserver/transactions/characterCreateRequest.hpp>

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
        << ", accountId: " << request_.getAccountId();

    std::stringstream query;
    query << "SELECT hairColor, hairType, level, name, race, tutorialState FROM characters WHERE accountId='" << request_.getAccountId() << "';";

    if(!sqlInterface_.executeQuery(query.str()))
    {
        this->sendFaultIndication(sqlInterface_.getErrorMessage());
        return;
    }
}

}
}
}

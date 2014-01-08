#include <gameserver/transactions/charactersListResponse.hpp>
#include <streaming/gameserver/charactersListResponse.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

CharactersListResponse::CharactersListResponse(core::common::Factory<User> &usersFactory,
                                               const streaming::dataserver::CharactersListResponse &response):
    usersFactory_(usersFactory),
    response_(response) {}

bool CharactersListResponse::isValid() const
{
    return usersFactory_.exists(response_.getUserHash());
}

void CharactersListResponse::handleValid()
{
    User &user = usersFactory_.find(response_.getUserHash());

    eMU_LOG(info) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId()
        << ", characters list response, #characters: " << response_.getCharacters().size();

    streaming::gameserver::CharactersListResponse response(response_.getCharacters());
    user.getConnection().send(response.getWriteStream().getPayload());
}

void CharactersListResponse::handleInvalid()
{
    eMU_LOG(error) << "hash: " << response_.getUserHash() << ", given in dataserver response does not exist!";
}

}
}
}

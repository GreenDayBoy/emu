#include <gameserver/transactions/characterCreateResponse.hpp>
#include <streaming/gameserver/characterCreateResponse.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

CharacterCreateResponse::CharacterCreateResponse(core::common::Factory<User> &usersFactory,
                                                 const streaming::dataserver::CharacterCreateResponse &response):
    usersFactory_(usersFactory),
    response_(response) {}

bool CharacterCreateResponse::isValid() const
{
    return usersFactory_.exists(response_.getUserHash());
}

void CharacterCreateResponse::handleValid()
{   
    streaming::gameserver::CharacterCreateResult result = streaming::gameserver::CharacterCreateResult::Succeed;

    switch(response_.getResult())
    {
    case streaming::dataserver::CharacterCreateResult::AlreadyExists:
        result = streaming::gameserver::CharacterCreateResult::AlreadyExists;
        break;
    case streaming::dataserver::CharacterCreateResult::CharactersCountExceeded:
        result = streaming::gameserver::CharacterCreateResult::CharactersCountExceeded;
        break;
    default:
        break;
    }

    User &user = usersFactory_.find(response_.getUserHash());

    eMU_LOG(info) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId()
        << ", dataserver result: " << static_cast<uint32_t>(response_.getResult())
        << ", result: " << static_cast<uint32_t>(result);

    user.getConnection().send(streaming::gameserver::CharacterCreateResponse(result).getWriteStream().getPayload());
}

void CharacterCreateResponse::handleInvalid()
{
    eMU_LOG(error) << "hash: " << response_.getUserHash() << ", given in dataserver response does not exist!";
}

}
}
}

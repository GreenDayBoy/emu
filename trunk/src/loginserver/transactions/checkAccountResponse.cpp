#include <loginserver/transactions/checkAccountResponse.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/loginserver/loginResponse.hpp>
#include <streaming/loginserver/loginResult.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

CheckAccountResponse::CheckAccountResponse(core::common::Factory<User> &usersFactory,
                                           const streaming::dataserver::CheckAccountResponse &response):
    usersFactory_(usersFactory),
    response_(response) {}

bool CheckAccountResponse::isValid() const
{
    return usersFactory_.exists(response_.getUserHash());
}

void CheckAccountResponse::handleValid()
{
    streaming::loginserver::LoginResult result = streaming::loginserver::LoginResult::Succeed;

    switch(response_.getResult())
    {
    case streaming::dataserver::CheckAccountResult::AccountInUse:
        result = streaming::loginserver::LoginResult::AccountInUse;
        break;
    case streaming::dataserver::CheckAccountResult::InvalidAccountId:
        result =  streaming::loginserver::LoginResult::InvalidAccountId;
        break;
    case streaming::dataserver::CheckAccountResult::InvalidPassword:
        result = streaming::loginserver::LoginResult::InvalidPassword;
        break;
    default:
        break;
    }

    User &user = usersFactory_.find(response_.getUserHash());

    eMU_LOG(info) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId()
        << ", check account result: " << static_cast<uint32_t>(response_.getResult())
        << ", login result: " << static_cast<uint32_t>(result);

    streaming::loginserver::LoginResponse response(result);
    user.getConnection().send(response.getWriteStream().getPayload());
}

void CheckAccountResponse::handleInvalid()
{
    eMU_LOG(error) << "hash: " << response_.getUserHash() << ", given in dataserver response does not exist!";
}

}
}
}

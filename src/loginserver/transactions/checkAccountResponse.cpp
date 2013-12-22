#include <loginserver/transactions/checkAccountResponse.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/loginserver/loginResponse.hpp>
#include <protocol/loginserver/loginResult.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

CheckAccountResponse::CheckAccountResponse(core::common::Factory<User> &usersFactory,
                                           const protocol::dataserver::CheckAccountResponse &response):
    usersFactory_(usersFactory),
    response_(response) {}

bool CheckAccountResponse::isValid() const
{
    bool result = true;

    try
    {
        usersFactory_.find(response_.getClientHash());
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        result = false;
    }

    return result;
}

void CheckAccountResponse::handleValid()
{
    User &user = usersFactory_.find(response_.getClientHash());

    protocol::loginserver::LoginResult result = protocol::loginserver::LoginResult::Succeed;

    if(response_.getResult() == protocol::dataserver::CheckAccountResult::AccountInUse)
    {
        result = protocol::loginserver::LoginResult::AccountInUse;
    }
    else if(response_.getResult() == protocol::dataserver::CheckAccountResult::InvalidAccountId)
    {
        result = protocol::loginserver::LoginResult::InvalidAccountId;
    }
    else if(response_.getResult() == protocol::dataserver::CheckAccountResult::InvalidPassword)
    {
        result = protocol::loginserver::LoginResult::InvalidPassword;
    }

    LOG(INFO) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId()
              << ", check account result: " << static_cast<uint32_t>(response_.getResult())
              << ", login result: " << static_cast<uint32_t>(result);

    protocol::loginserver::LoginResponse response(result);
    user.getConnection().send(response.getWriteStream().getPayload());
}

void CheckAccountResponse::handleInvalid()
{
    LOG(ERROR) << "hash: " << response_.getClientHash() << " given in dataserver response does not exist!";
}

}
}
}

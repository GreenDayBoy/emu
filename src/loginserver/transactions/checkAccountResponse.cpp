#include <loginserver/transactions/checkAccountResponse.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <streaming/loginserver/loginResponse.hpp>
#include <streaming/loginserver/loginResult.hpp>

#include <glog/logging.h>

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

    streaming::loginserver::LoginResult result = streaming::loginserver::LoginResult::Succeed;

    if(response_.getResult() == streaming::dataserver::CheckAccountResult::AccountInUse)
    {
        result = streaming::loginserver::LoginResult::AccountInUse;
    }
    else if(response_.getResult() == streaming::dataserver::CheckAccountResult::InvalidAccountId)
    {
        result = streaming::loginserver::LoginResult::InvalidAccountId;
    }
    else if(response_.getResult() == streaming::dataserver::CheckAccountResult::InvalidPassword)
    {
        result = streaming::loginserver::LoginResult::InvalidPassword;
    }

    LOG(INFO) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId()
              << ", check account result: " << static_cast<uint32_t>(response_.getResult())
              << ", login result: " << static_cast<uint32_t>(result);

    streaming::loginserver::LoginResponse response(result);
    user.getConnection().send(response.getWriteStream().getPayload());
}

void CheckAccountResponse::handleInvalid()
{
    LOG(ERROR) << "hash: " << response_.getClientHash() << " given in dataserver response does not exist!";
}

}
}
}

#include <loginserver/transactions/checkAccountResponseTransaction.hpp>
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

CheckAccountResponseTransaction::CheckAccountResponseTransaction(core::network::tcp::ConnectionsManager &connectionsManager,
                                                                 core::common::UsersFactory<User> &usersFactory,
                                                                 const protocol::dataserver::CheckAccountResponse &response):
    connectionsManager_(connectionsManager),
    usersFactory_(usersFactory),
    response_(response) {}

bool CheckAccountResponseTransaction::isValid() const
{
    return usersFactory_.exists(response_.getClientHash());
}

void CheckAccountResponseTransaction::handleValid()
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
    connectionsManager_.send(user.getHash(), response.getWriteStream().getPayload());
}

void CheckAccountResponseTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << response_.getClientHash() << " given in dataserver response does not exist!";
}

}
}
}

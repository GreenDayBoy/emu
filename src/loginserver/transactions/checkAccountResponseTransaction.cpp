#include <loginserver/transactions/checkAccountResponseTransaction.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/loginserver/encoders/loginResponse.hpp>
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
                                                                 const protocol::dataserver::decoders::CheckAccountResponse &response):
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

    LOG(INFO) << "hash: " << user.getHash() << ", accountId: " << user.getAccountId() << ", check account result: " << static_cast<uint32_t>(response_.getResult());

    protocol::loginserver::LoginResult result = protocol::loginserver::LoginResult::Failed;

    if(response_.getResult() == protocol::dataserver::CheckAccountResult::Succeed)
    {
        result = protocol::loginserver::LoginResult::Succeed;
    }

    protocol::loginserver::encoders::LoginResponse response(result);
    connectionsManager_.send(user.getHash(), response.getWriteStream().getPayload());
}

void CheckAccountResponseTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << response_.getClientHash() << " given in dataserver response does not exist!";
}

}
}
}

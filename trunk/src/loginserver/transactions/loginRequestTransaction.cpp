#include <loginserver/transactions/loginRequestTransaction.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

LoginRequestTransaction::LoginRequestTransaction(User &user,
                                                 core::network::tcp::ConnectionsManager &connectionsManager,
                                                 core::network::tcp::Connection &dataserverConnection,
                                                 const protocol::loginserver::LoginRequest &request):
    user_(user),
    connectionsManager_(connectionsManager),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool LoginRequestTransaction::isValid() const
{
    return dataserverConnection_.isOpen();
}

void LoginRequestTransaction::handleValid()
{
    user_.setAccountId(request_.getAccountId());

    LOG(INFO) << "hash: " << user_.getHash() << ", accountId: " << user_.getAccountId() << ", sending request for account check.";

    protocol::dataserver::CheckAccountRequest checkAccountRequest(user_.getHash(), request_.getAccountId(), request_.getPassword());
    dataserverConnection_.send(checkAccountRequest.getWriteStream().getPayload());
}

void LoginRequestTransaction::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", accountId: " << request_.getAccountId()
               << ", connection to dataserver not established! Disconnected.";

    connectionsManager_.disconnect(user_.getHash());
}

}
}
}

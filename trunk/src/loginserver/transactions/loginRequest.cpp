#include <loginserver/transactions/loginRequest.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

LoginRequest::LoginRequest(User &user,
                           core::network::tcp::Connection::Pointer dataserverConnection,
                           const protocol::loginserver::LoginRequest &request):
    user_(user),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool LoginRequest::isValid() const
{
    return dataserverConnection_ != nullptr && dataserverConnection_->isOpen();
}

void LoginRequest::handleValid()
{
    user_.setAccountId(request_.getAccountId());

    LOG(INFO) << "hash: " << user_.getHash() << ", accountId: " << user_.getAccountId() << ", sending request for account check.";

    protocol::dataserver::CheckAccountRequest checkAccountRequest(user_.getHash(), request_.getAccountId(), request_.getPassword());
    dataserverConnection_->send(checkAccountRequest.getWriteStream().getPayload());
}

void LoginRequest::handleInvalid()
{
    LOG(ERROR) << "hash: " << user_.getHash() << ", accountId: " << request_.getAccountId()
               << ", connection to dataserver not established! Disconnected.";

    user_.getConnection().disconnect();
}

}
}
}

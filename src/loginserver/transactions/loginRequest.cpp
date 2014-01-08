#include <loginserver/transactions/loginRequest.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

LoginRequest::LoginRequest(User &user,
                           core::network::tcp::Connection::Pointer dataserverConnection,
                           const streaming::loginserver::LoginRequest &request):
    user_(user),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool LoginRequest::isValid() const
{
    return dataserverConnection_ != nullptr && dataserverConnection_->isOpen();
}

void LoginRequest::handleValid()
{
    eMU_LOG(info) << "hash: " << user_.getHash() << ", accountId: " << request_.getAccountId();
    user_.setAccountId(request_.getAccountId());

    eMU_LOG(info) << "hash: " << user_.getHash() << ", sending request for account check.";
    streaming::dataserver::CheckAccountRequest checkAccountRequest(user_.getHash(), request_.getAccountId(), request_.getPassword());
    dataserverConnection_->send(checkAccountRequest.getWriteStream().getPayload());
}

void LoginRequest::handleInvalid()
{
    eMU_LOG(error) << "hash: " << user_.getHash() << ", accountId: " << request_.getAccountId()
        << ", connection to dataserver not established! Disconnected.";

    user_.getConnection().disconnect();
}

}
}
}

#include <gameserver/transactions/charactersListRequest.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

CharactersListRequest::CharactersListRequest(User &user,
                                             core::network::tcp::Connection::Pointer dataserverConnection,
                                             const streaming::gameserver::CharactersListRequest &request):
    user_(user),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool CharactersListRequest::isValid() const
{
    return dataserverConnection_ != nullptr && dataserverConnection_->isOpen();
}

void CharactersListRequest::handleValid()
{
    eMU_LOG(info) << "Sending characters list request, hash: " << user_.getHash()
        << ", accountId: " << user_.getAccountId();

    streaming::dataserver::CharactersListRequest request(user_.getHash(), user_.getAccountId());
    dataserverConnection_->send(request.getWriteStream().getPayload());
}

void CharactersListRequest::handleInvalid()
{
    eMU_LOG(error) << "hash: " << user_.getHash() << ", accountId: " << user_.getAccountId()
        << ", connection to dataserver not established! Disconnected.";

    user_.getConnection().disconnect();
}

}
}
}

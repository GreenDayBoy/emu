#include <gameserver/transactions/characterCreateRequest.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

CharacterCreateRequest::CharacterCreateRequest(User &user,
                                               core::network::tcp::Connection::Pointer dataserverConnection,
                                               const streaming::gameserver::CharacterCreateRequest &request):
    user_(user),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool CharacterCreateRequest::isValid() const
{
    return dataserverConnection_ != nullptr && dataserverConnection_->isOpen();
}

void CharacterCreateRequest::handleValid()
{
    eMU_LOG(info) << "Sending character create request, hash: " << user_.getHash()
        << ", accountId: " << user_.getAccountId()
        << ", character name: " << request_.getCharacterCreateInfo().name_;

    streaming::dataserver::CharacterCreateRequest request(user_.getHash(), user_.getAccountId(), request_.getCharacterCreateInfo());
    dataserverConnection_->send(request.getWriteStream().getPayload());
}

void CharacterCreateRequest::handleInvalid()
{
    eMU_LOG(error) << "hash: " << user_.getHash() << ", accountId: " << user_.getAccountId()
        << ", connection to dataserver not established! Disconnected.";

    user_.getConnection().disconnect();
}

}
}
}

#include <gameserver/transactions/registerUserRequest.hpp>
#include <streaming/gameserver/registerUserResponse.hpp>
#include <streaming/gameserver/userRegistrationResult.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

RegisterUserRequest::RegisterUserRequest(const boost::asio::ip::udp::endpoint &senderEndpoint,
                                         core::network::udp::Connection::Pointer udpConnection,
                                         streaming::gameserver::UserRegistrationInfoContainer &userRegistrationInfos,
                                         uint16_t gameserverCode,
                                         const streaming::gameserver::RegisterUserRequest &request):
    senderEndpoint_(senderEndpoint),
    udpConnection_(udpConnection),
    userRegistrationInfos_(userRegistrationInfos),
    gameserverCode_(gameserverCode),
    request_(request) {}

bool RegisterUserRequest::isValid() const
{
    return udpConnection_ != nullptr;
}

void RegisterUserRequest::handleValid()
{
    eMU_LOG(info) << "accountId: " << request_.getUserRegistrationInfo().accountId_
        << ", hash: " << request_.getUserRegistrationInfo().userHash_;

    streaming::gameserver::UserRegistrationResult result = streaming::gameserver::UserRegistrationResult::Succeed;

    if(std::find(userRegistrationInfos_.begin(),
                 userRegistrationInfos_.end(),
                 request_.getUserRegistrationInfo()) == userRegistrationInfos_.end())
    {
        userRegistrationInfos_.push_back(request_.getUserRegistrationInfo());
    }
    else
    {
        result = streaming::gameserver::UserRegistrationResult::Failed;
    }

    eMU_LOG(info) << "User registration result: " << static_cast<uint32_t>(result);

    streaming::gameserver::RegisterUserResponse response(gameserverCode_, request_.getUserRegistrationInfo().userHash_, result);
    udpConnection_->sendTo(senderEndpoint_, response.getWriteStream().getPayload());
}

void RegisterUserRequest::handleInvalid()
{
    eMU_LOG(error) << "udpConnection is nullptr!";
}

}
}
}

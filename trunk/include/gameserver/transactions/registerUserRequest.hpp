#pragma once

#include <core/common/transaction.hpp>
#include <core/network/udp/connection.hpp>
#include <streaming/gameserver/registerUserRequest.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class RegisterUserRequest: public core::common::Transaction
{
public:
    RegisterUserRequest(const boost::asio::ip::udp::endpoint &senderEndpoint,
                        core::network::udp::Connection::Pointer udpConnection,
                        streaming::gameserver::UserRegistrationInfoContainer &userRegistrationInfos,
                        uint16_t gameserverCode,
                        const streaming::gameserver::RegisterUserRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    const boost::asio::ip::udp::endpoint &senderEndpoint_;
    core::network::udp::Connection::Pointer udpConnection_;
    streaming::gameserver::UserRegistrationInfoContainer &userRegistrationInfos_;
    uint16_t gameserverCode_;
    streaming::gameserver::RegisterUserRequest request_;
};

}
}
}

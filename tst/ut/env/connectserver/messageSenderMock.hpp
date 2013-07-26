#pragma once

#include <gmock/gmock.h>
#include <connectserver/messageSender.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace connectserver
{

class MessageSenderMock: public eMU::connectserver::MessageSender
{
public:
    MessageSenderMock();

    MOCK_METHOD2(sendGameServersListResponse, void(size_t hash, const eMU::connectserver::GameServersList::GameServersListContainer &list));
    MOCK_METHOD3(sendGameServerAddressResponse, void(size_t hash, const std::string &address, uint16_t port));
};

}
}
}
}

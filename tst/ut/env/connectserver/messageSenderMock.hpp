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
    MOCK_METHOD2(sendGameServersListResponse, void(size_t hash, const eMU::connectserver::GameServersList::GameServersListContainer &list));
};

}
}
}
}

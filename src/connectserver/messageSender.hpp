#pragma once

#include <connectserver/gameServersList.hpp>
#include <common/mockable.hpp>

namespace eMU
{
namespace connectserver
{

class MessageSender
{
public:
    MOCKABLE void sendGameServersListResponse(size_t hash, const GameServersList::GameServersListContainer &servers);

private:
};

}
}

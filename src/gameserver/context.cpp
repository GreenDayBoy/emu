#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

Context::Context(size_t maxNumberOfUsers, uint16_t gameserverCode):
    protocols::contexts::Server<User>(maxNumberOfUsers),
    gameserverCode_(gameserverCode) {}

streaming::gameserver::UserRegistrationInfoContainer& Context::getUserRegistrationInfos()
{
    return userRegistrationInfos_;
}

uint16_t Context::getGameserverCode() const
{
    return gameserverCode_;
}

}
}

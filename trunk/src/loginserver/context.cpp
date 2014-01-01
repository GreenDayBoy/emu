#include <loginserver/context.hpp>

namespace eMU
{
namespace loginserver
{

Context::Context(size_t maxNumberOfUsers):
    protocols::contexts::Server<User>(maxNumberOfUsers) {}

GameserversList& Context::getGameserversList()
{
    return gameserversList_;
}

}
}

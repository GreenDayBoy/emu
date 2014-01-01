#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

Context::Context(size_t maxNumberOfUsers):
    protocols::contexts::Server<User>(maxNumberOfUsers) {}

}
}

#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{

User::User(core::network::tcp::Connection &connection):
    NetworkUser(connection) {}

}
}

#include <dataserver/user.hpp>

namespace eMU
{
namespace dataserver
{

User::User(core::network::tcp::Connection::Pointer connection):
    NetworkUser(connection) {}

}
}

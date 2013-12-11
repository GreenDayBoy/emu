#include <dataserver/user.hpp>

namespace eMU
{
namespace dataserver
{

User::User(core::network::tcp::Connection &connection):
    NetworkUser(connection) {}

}
}

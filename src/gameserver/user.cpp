#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{

User::User(core::network::tcp::Connection::Pointer connection):
    NetworkUser(connection) {}

void User::setAccountId(const std::string &accountId)
{
    accountId_ = accountId;
}

const std::string& User::getAccountId() const
{
    return accountId_;
}

}
}

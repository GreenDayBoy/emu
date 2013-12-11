#include <loginserver/user.hpp>

namespace eMU
{
namespace loginserver
{

User::User(core::network::tcp::Connection &connection):
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

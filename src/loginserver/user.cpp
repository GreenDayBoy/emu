#include <loginserver/user.hpp>

namespace eMU
{
namespace loginserver
{

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

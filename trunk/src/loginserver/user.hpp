#pragma once

#include <core/network/tcp/networkUser.hpp>
#include <string>

namespace eMU
{
namespace loginserver
{

class User: public core::network::tcp::NetworkUser
{
public:
    User(core::network::tcp::Connection &connection);

    void setAccountId(const std::string &accountId);
    const std::string& getAccountId() const;

private:
    User();

    std::string accountId_;
};

}
}

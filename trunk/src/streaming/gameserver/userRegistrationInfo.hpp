#pragma once

#include <core/network/tcp/networkUser.hpp>
#include <string>
#include <deque>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

struct UserRegistrationInfo
{
    core::network::tcp::NetworkUser::Hash userHash_;
    std::string accountId_;

    bool operator==(const UserRegistrationInfo &info)
    {
        return info.accountId_ == accountId_ && info.userHash_ == userHash_;
    }
};

typedef std::deque<UserRegistrationInfo> UserRegistrationInfoContainer;

}
}
}

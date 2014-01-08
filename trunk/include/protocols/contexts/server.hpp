#pragma once

#include <core/common/factory.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

template<typename UserType>
class Server: boost::noncopyable
{
public:
    Server(size_t maxNumberOfUsers):
        maxNumberOfUsers_(maxNumberOfUsers) {}

    virtual ~Server() {}

    core::common::Factory<UserType>& getUsersFactory()
    {
        return usersFactory_;
    }

    size_t getMaxNumberOfUsers()
    {
        return maxNumberOfUsers_;
    }

protected:
    Server();

    core::common::Factory<UserType> usersFactory_;
    size_t maxNumberOfUsers_;
};

}
}
}

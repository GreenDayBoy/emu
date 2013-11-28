#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include <core/common/exception.hpp>

namespace eMU
{
namespace core
{
namespace common
{

template<typename User>
class UsersFactory
{
public:
    typedef std::vector<User*> UsersContainer;
    typedef std::shared_ptr<UsersFactory<User> > Pointer;

    class MaxNumberOfUsersReachedException: public Exception {};
    class UnknownUserException: public Exception {};

    UsersFactory(size_t maxNumberOfUsers):
        maxNumberOfUsers_(maxNumberOfUsers) {}

    size_t create()
    {
        if(users_.size() >= maxNumberOfUsers_)
        {
            throw MaxNumberOfUsersReachedException();
        }

        User *user = new User();
        users_.push_back(user);
        return user->getHash();
    }

    void destroy(size_t hash)
    {
        typename UsersContainer::iterator it;
        it = std::find_if(users_.begin(),
                          users_.end(),
                          [&hash](const User *userFromVector)
        {
            return hash == userFromVector->getHash();
        });

        if(it != users_.end())
        {
            delete *it;
            users_.erase(it);
        }
    }

    User& find(size_t hash)
    {
        typename UsersContainer::iterator it;
        it = std::find_if(users_.begin(),
                          users_.end(),
                          [hash](const User *userFromVector) { return hash == userFromVector->getHash(); });

        if(it == users_.end())
        {
            throw UnknownUserException();
        }

        return *(*it);
    }

    UsersContainer& users()
    {
        return users_;
    }

    bool exists(size_t hash) const
    {
        size_t count = std::count_if(users_.begin(),
                                     users_.end(),
                                     [hash](const User *userFromVector) { return hash == userFromVector->getHash(); });

        return count > 0;
    }

private:
    size_t maxNumberOfUsers_;
    UsersContainer users_;
};

}
}
}

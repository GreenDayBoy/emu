#pragma once

#include <vector>
#include <algorithm>
#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace common {

template<typename User>
class UsersFactory {
public:
    typedef std::vector<User*> UsersContainer;

    UsersFactory(size_t maxNumberOfUsers):
      maxNumberOfUsers_(maxNumberOfUsers) {}

    User& create() {
        if(users_.size() >= maxNumberOfUsers_) {
            Exception exception;
            exception.in() << "Max number of users reached!";

            throw exception;
        }

        User *user = new User();
        users_.push_back(user);
        return *user;
    }

    void destroy(size_t hash) {
        typename UsersContainer::iterator it;
         it = std::find_if(users_.begin(),
                           users_.end(),
                           [&hash](const User *userFromVector) { return hash == userFromVector->hash(); });

        if(it != users_.end())
        {
            delete *it;
            users_.erase(it);
        }
    }

    User& find(size_t hash) {
        typename UsersContainer::iterator it;
        it = std::find_if(users_.begin(),
                          users_.end(),
                          [hash](const User *userFromVector) { return hash == userFromVector->hash(); });

        if(it == users_.end()) {
            Exception exception;
            exception.in() << "Max number of users reached!";

            throw exception;
        }

        return *(*it);
    }

    UsersContainer& users() { return users_; }

private:
    size_t maxNumberOfUsers_;
    UsersContainer users_;
};

}
}
}

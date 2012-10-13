#pragma once


#include <common/objectsFactory.hpp>
#include <common/log.hpp>
#include <common/exception.hpp>
#include <user/userIdGenerator.hpp>

namespace eMU {
namespace core {
namespace user {

template<typename userImpl>
class factory_t: public objectsFactory_t<userImpl> {
public:
    factory_t(size_t maxNumOfUsers):
      idGenerator_(maxNumOfUsers) {}

    userImpl& construct() {
        int16 id = idGenerator_.get();

        userImpl *user = this->objectsPool_.construct(id);

        if(NULL == user) {
            throw exception_t("Error in allocating new user object!");
        }

        return *user;
    }

    void destroy(userImpl &user) {
        int16 id = user.id();
        idGenerator_.insert(id);
        this->objectsPool_.destroy(&user);
    }

protected:
    idGenerator_t idGenerator_;
};

}
}
}

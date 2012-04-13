#ifndef eMU_CORE_USERSFACTORY_HPP
#define eMU_CORE_USERSFACTORY_HPP

#include "objectsFactory.hpp"
#include "userIdGenerator.hpp"
#include "log.hpp"

namespace eMU {
namespace core {
namespace user {

template<typename UserImpl>
class factory_t: objectsFactory_t<UserImpl> {
public:
    factory_t(size_t maxNumOfUsers):
      idGenerator_(maxNumOfUsers) {}

    UserImpl* construct() {
        int16 id = idGenerator_.get();

        if(id == invalidId_c) {
            LOG_ERROR << "Generated invalid id!" << std::endl;
            return NULL;
        }

        UserImpl *user = objectsPool_.construct(id);
        return user;
    }

    void destroy(UserImpl *user) {
        int16 id = user->id();
        idGenerator_.insert(id);
        objectsPool_.destroy(user);
    }

protected:
    idGenerator_t idGenerator_;
};

}
}
}

#endif
#ifndef eMU_CORE_USERFACTORY_HPP
#define eMU_CORE_USERFACTORY_HPP

#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>
#include "userIdGenerator.hpp"
#include "log.hpp"

namespace eMU {
namespace core {
namespace user {

template<typename UserImpl>
class factory_t:private boost::noncopyable {
public:
    factory_t(size_t maxNumOfUsers):
      idGenerator_(maxNumOfUsers) {}

    UserImpl* construct() {
        int16 id = idGenerator_.get();

        if(id == invalidId_c) {
            LOG_ERROR << "Generated invalid id!" << std::endl;
            return NULL;
        }

        UserImpl *user = pool_.construct(id);
        return user;
    }

    void destroy(UserImpl *user) {
        int16 id = user->id();
        idGenerator_.insert(id);
        pool_.destroy(user);
    }

protected:
    boost::object_pool<UserImpl> pool_;
    idGenerator_t idGenerator_;
};

}
}
}

#endif
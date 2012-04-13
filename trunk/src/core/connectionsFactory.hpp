#ifndef eMU_CORE_CONNECTIONSFACTORY_HPP
#define eMU_CORE_CONNECTIONSFACTORY_HPP

#include "objectsFactory.hpp"

namespace eMU {
namespace core {
namespace network {

template<typename ConnectionImpl,
         typename IoServiceImpl>
class connectionsFactory_t: objectsFactory_t<ConnectionImpl> {
public:
    connectionsFactory_t(IoServiceImpl &ioService):
      ioService_(ioService) {}

    ConnectionImpl* construct(typename ConnectionImpl::observer_i &observer) {
        return objectsPool_.construct(boost::ref(ioService_), boost::ref(observer));
    }

    void destroy(ConnectionImpl *connection) {
        connection->close();

        if(objectsPool_.is_from(connection)) {
            objectsPool_.destroy(connection);
        }
    }

protected:
    connectionsFactory_t();

    IoServiceImpl &ioService_;
};

}
}
}

#endif
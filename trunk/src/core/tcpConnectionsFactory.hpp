#ifndef eMU_CORE_TCPCONNECTIONSFACTORY_HPP
#define eMU_CORE_TCPCONNECTIONSFACTORY_HPP

#include "objectsFactory.hpp"

namespace eMU {
namespace core {
namespace network {
namespace tcp {

template<typename ConnectionImpl,
         typename IoServiceImpl>
class connectionsFactory_t: public objectsFactory_t<ConnectionImpl> {
public:
    connectionsFactory_t(IoServiceImpl &ioService):
      ioService_(ioService) {}

    ConnectionImpl* construct() {
        return objectsPool_.construct(boost::ref(ioService_));
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
}

#endif
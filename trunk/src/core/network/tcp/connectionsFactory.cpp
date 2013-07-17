#include <network/tcp/connectionsFactory.hpp>
#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace network {
namespace tcp {

Connection& ConnectionsFactory::create(size_t hash, Connection::SocketPointer socket) {
    if(connections_.count(hash) > 0) {
        eMU::core::common::Exception exception;
        exception.in() << "Connection with hash: " << hash << " has already been created! Connection: " << *connections_[hash];

        throw exception;
    }

    Connection::Pointer connection(new Connection(socket));
    connections_[hash] = connection;

    return *connection;
}

void ConnectionsFactory::destroy(size_t hash) {
    if(connections_.count(hash) == 0) {
        eMU::core::common::Exception exception;
        exception.in() << "Connection with hash: " << hash << " does not exist!";

        throw exception;
    }

    connections_[hash].reset();
    connections_.erase(hash);
}

Connection& ConnectionsFactory::get(size_t hash) {
    if(connections_.count(hash) == 0) {
        eMU::core::common::Exception exception;
        exception.in() << "Connection with hash: " << hash << " does not exist!";

        throw exception;
    }

    return *connections_[hash];
}

size_t ConnectionsFactory::getHash(Connection &connection) const {
    ConnectionsContainer::const_iterator it = connections_.begin();

    for(; it != connections_.end(); it++) {
        if(it->second->hash() == connection.hash()) {
            return it->first;
        }
    }

    common::Exception exception;
    exception.in() << "Could not find connection, hash: " << connection.hash() << ", connection: " << connection;

    throw exception;
}

}
}
}
}

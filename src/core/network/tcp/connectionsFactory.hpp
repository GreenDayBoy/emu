#pragma once

#include <boost/noncopyable.hpp>
#include <network/tcp/connection.hpp>

namespace eMU {
namespace core {
namespace network {
namespace tcp {

class ConnectionsFactory: boost::noncopyable {
public:
    typedef std::shared_ptr<ConnectionsFactory> Pointer;
    typedef std::map<size_t, Connection::Pointer> ConnectionsContainer;

    Connection& create(size_t hash, Connection::SocketPointer socket);
    void destroy(size_t hash);
    Connection& get(size_t hash);
    size_t getHash(Connection &connection) const;

private:
    ConnectionsContainer connections_;
};

}
}
}
}

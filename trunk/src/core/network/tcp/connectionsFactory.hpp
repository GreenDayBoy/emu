#pragma once

#include <boost/noncopyable.hpp>
#include <core/network/tcp/connection.hpp>
#include <common/mockable.hpp>
#include <core/common/exception.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class ConnectionsFactory: boost::noncopyable
{
public:
    typedef std::shared_ptr<ConnectionsFactory> Pointer;
    typedef std::map<size_t, Connection::Pointer> ConnectionsContainer;

    class UnknownConnectionException: public common::Exception {};
    class AlreadyExistingConnectionException: public common::Exception {};

    virtual ~ConnectionsFactory();

    MOCKABLE Connection& create(size_t hash, Connection::SocketPointer socket);
    MOCKABLE void destroy(size_t hash);
    MOCKABLE Connection& get(size_t hash);
    MOCKABLE size_t getHash(Connection &connection) const;

private:
    ConnectionsContainer connections_;
};

}
}
}
}

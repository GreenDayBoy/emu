#pragma once

#include <core/network/tcp/connection.hpp>
#include <boost/strong_typedef.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class NetworkUser: boost::noncopyable
{
public:
    BOOST_STRONG_TYPEDEF(size_t, Hash)

    NetworkUser(Connection::Pointer connection);
    virtual ~NetworkUser();

    Connection& getConnection();
    bool operator==(const Connection::Pointer connection) const;
    bool operator==(Hash hash) const;
    bool operator==(const NetworkUser &user) const;

    Hash getHash() const;

protected:
    NetworkUser();

    Connection::Pointer connection_;
};

}
}
}
}

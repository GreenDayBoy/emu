#pragma once

#include <boost/noncopyable.hpp>
#include <core/network/tcp/connection.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class Protocol: boost::noncopyable
{
public:
    virtual ~Protocol();
    virtual bool attach(Connection::Pointer connection);
    virtual void detach(Connection::Pointer connection);
    virtual bool dispatch(Connection::Pointer connection);

    virtual void shutdown();
};

}
}
}
}

#pragma once

#include <boost/noncopyable.hpp>
#include <core/common/asio.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

class Connection;

class Protocol: boost::noncopyable
{
public:
    virtual ~Protocol();
    virtual void attach(Connection& connection);
    virtual void dispatch(Connection& connection, const boost::asio::ip::udp::endpoint &senderEndpoint);
    virtual void detach(Connection& connection);
    virtual void shutdown();
};

}
}
}
}

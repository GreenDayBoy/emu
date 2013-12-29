#pragma once

#include <boost/noncopyable.hpp>
#include <common/asio.hpp>

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
    virtual void dispatch(Connection& connection, const boost::asio::ip::udp::endpoint &senderEndpoint);
    virtual void shutdown();
};

}
}
}
}

#pragma once

#include <core/network/udp/connection.hpp>
#include <core/common/asio.hpp>
#include <boost/noncopyable.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

class Protocol: boost::noncopyable
{
public:
    virtual ~Protocol();
    virtual void attach(Connection::Pointer connection);
    virtual void dispatch(Connection::Pointer connection, const boost::asio::ip::udp::endpoint &senderEndpoint);
    virtual void detach(Connection::Pointer connection);
    virtual void shutdown();
};

}
}
}
}

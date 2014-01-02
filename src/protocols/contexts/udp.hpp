#pragma once

#include <core/network/udp/connection.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

class Udp: boost::noncopyable
{
public:
    Udp();
    virtual ~Udp();

    void setUdpConnection(core::network::udp::Connection::Pointer connection);
    core::network::udp::Connection::Pointer getUdpConnection();

protected:
    core::network::udp::Connection::Pointer udpConnection_;
};

}
}
}

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

    void setUdpConnection(core::network::udp::Connection *connection);
    core::network::udp::Connection* getUdpConnection();

protected:
    core::network::udp::Connection* udpConnection_;
};

}
}
}

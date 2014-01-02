#include <protocols/contexts/udp.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

Udp::Udp():
    udpConnection_(nullptr) {}

Udp::~Udp() {}

void Udp::setUdpConnection(core::network::udp::Connection *connection)
{
    udpConnection_ = connection;
}

core::network::udp::Connection* Udp::getUdpConnection()
{
    return udpConnection_;
}

}
}
}

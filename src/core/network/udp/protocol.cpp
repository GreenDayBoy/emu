#include <core/network/udp/protocol.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

Protocol::~Protocol()
{

}

void Protocol::attach(Connection&)
{

}

void Protocol::dispatch(Connection&, const boost::asio::ip::udp::endpoint&)
{

}

void Protocol::detach(Connection&)
{

}

void Protocol::shutdown()
{

}

}
}
}
}

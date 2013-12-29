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

void Protocol::dispatch(Connection&, const boost::asio::ip::udp::endpoint&)
{

}

void Protocol::shutdown()
{

}

}
}
}
}

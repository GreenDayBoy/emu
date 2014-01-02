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

void Protocol::attach(Connection::Pointer)
{

}

void Protocol::dispatch(Connection::Pointer, const boost::asio::ip::udp::endpoint&)
{

}

void Protocol::detach(Connection::Pointer)
{

}

void Protocol::shutdown()
{

}

}
}
}
}

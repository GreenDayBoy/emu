#include <core/network/udp/writeBufferFactory.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

WriteBuffer& WriteBufferFactory::get(const boost::asio::ip::udp::endpoint &endpoint)
{
    if(buffers_.count(endpoint) > 0)
    {
        return buffers_[endpoint];
    }
    else
    {
        buffers_[endpoint] = WriteBuffer();
        return buffers_[endpoint];
    }
}

bool WriteBufferFactory::erase(const boost::asio::ip::udp::endpoint &endpoint)
{
    return buffers_.erase(endpoint) > 0 ? true: false;
}

}
}
}
}

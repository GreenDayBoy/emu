#pragma once

#include <core/network/writeBuffer.hpp>

#include <boost/asio/ip/udp.hpp>
#include <map>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

class WriteBufferFactory
{
public:
    WriteBuffer& get(const boost::asio::ip::udp::endpoint &endpoint);
    bool erase(const boost::asio::ip::udp::endpoint &endpoint);

private:
    std::map<boost::asio::ip::udp::endpoint, WriteBuffer> buffers_;
};

}
}
}
}

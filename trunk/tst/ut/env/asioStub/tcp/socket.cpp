#include <ut/env/asioStub/tcp/socket.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace asioStub
{
namespace ip
{
namespace tcp
{

socket::socket(io_service &service):
    service_(service) {}

io_service& socket::get_io_service()
{
    return service_;
}

boost::asio::ip::tcp::endpoint socket::remote_endpoint() const
{
    return boost::asio::ip::tcp::endpoint();
}

}
}
}
}
}
}

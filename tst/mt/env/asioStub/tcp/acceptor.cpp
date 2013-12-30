#include <mt/env/asioStub/tcp/acceptor.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{
namespace ip
{
namespace tcp
{

acceptor::acceptor(io_service &ioService, const boost::asio::ip::tcp::endpoint &endpoint):
    ioService_(ioService) {}

void acceptor::async_accept(socket &socket, const AcceptHandler &handler)
{

}

io_service& acceptor::get_io_service()
{
    return ioService_;
}

}
}
}
}
}
}

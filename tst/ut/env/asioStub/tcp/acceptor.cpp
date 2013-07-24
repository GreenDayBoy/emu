#include <ut/env/asioStub/tcp/acceptor.hpp>

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

acceptor::acceptor(io_service &ioService, const boost::asio::ip::tcp::endpoint &endpoint):
    ioService_(ioService) {}

}
}
}
}
}
}

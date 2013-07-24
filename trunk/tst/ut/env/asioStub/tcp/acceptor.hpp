#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <ut/env/asioStub/ioService.hpp>
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

class acceptor
{
public:
    typedef std::function<void(const boost::system::error_code&)> AcceptHandler;

    acceptor(io_service &ioService, const boost::asio::ip::tcp::endpoint &endpoint);
    MOCK_METHOD2(async_accept, void(socket &socket, const AcceptHandler &handler));

    io_service& ioService_;
};

}
}
}
}
}
}

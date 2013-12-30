#pragma once

#include <mt/env/asioStub/ioService.hpp>

#include <boost/asio.hpp>
#include <gmock/gmock.h>

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

class socket;

class acceptor
{
public:
    typedef std::function<void(const boost::system::error_code&)> AcceptHandler;

    acceptor(io_service &ioService, const boost::asio::ip::tcp::endpoint &endpoint);
    void async_accept(socket &socket, const AcceptHandler &handler);

    io_service& get_io_service();

private:
    io_service& ioService_;
};

}
}
}
}
}
}

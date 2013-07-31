#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include <mt/env/asioStub/types.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class io_service;

namespace ip
{
namespace tcp
{

class socket;

class acceptor
{
public:
    acceptor(io_service &ioService, const boost::asio::ip::tcp::endpoint &endpoint);
    void async_accept(socket &socket, const AcceptHandler &handler);

    io_service& ioService_;
};

}
}
}
}
}
}

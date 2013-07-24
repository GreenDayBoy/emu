#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <mt/env/asioStub/ioService.hpp>

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
namespace udp
{

class socket
{
public:
    socket(io_service &service, const boost::asio::ip::udp::endpoint &endpoint);

    void async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                            boost::asio::ip::udp::endpoint &senderEndpoint,
                            const io_service::IoHandler &handler);

    void async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                       boost::asio::ip::udp::endpoint endpoint,
                       const io_service::IoHandler &handler);

    io_service& get_io_service();

    io_service service_;
    boost::asio::ip::udp::endpoint endpoint_;
};

}
}
}
}
}
}

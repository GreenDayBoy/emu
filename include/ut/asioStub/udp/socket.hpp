#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <ut/asioStub/ioService.hpp>

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
namespace udp
{

class socket
{
public:
    socket(io_service &service, const boost::asio::ip::udp::endpoint &endpoint);

    MOCK_METHOD3(async_receive_from, void(const boost::asio::mutable_buffers_1 &buffer,
                                          boost::asio::ip::udp::endpoint &senderEndpoint,
                                          const io_service::IoHandler &handler));

    MOCK_METHOD3(async_send_to, void(const boost::asio::mutable_buffers_1 &buffer,
                                     boost::asio::ip::udp::endpoint endpoint,
                                     const io_service::IoHandler &handler));

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

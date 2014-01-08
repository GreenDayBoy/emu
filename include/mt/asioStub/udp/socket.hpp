#pragma once

#include <mt/asioStub/baseSocket.hpp>

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

class socket: public BaseSocket
{
public:
    socket(io_service &ioService, const boost::asio::ip::udp::endpoint&);

    void async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                            boost::asio::ip::udp::endpoint &senderEndpoint,
                            const io_service::IoHandler &handler);

    void async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                       boost::asio::ip::udp::endpoint endpoint,
                       const io_service::IoHandler &handler);
};

}
}
}
}
}
}

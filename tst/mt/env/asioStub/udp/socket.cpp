#include <mt/env/asioStub/udp/socket.hpp>
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

socket::socket(io_service &ioService, const boost::asio::ip::udp::endpoint&):
    BaseSocket(ioService) {}

void socket::async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                                boost::asio::ip::udp::endpoint &senderEndpoint,
                                const IoHandler &handler)
{
    receiveBuffer_ = buffer;
    receiveHandler_ = handler;

    ioService_.registerUdpSocket(this);
}

void socket::async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                           boost::asio::ip::udp::endpoint endpoint,
                           const IoHandler &handler)
{
    sendBuffer_ = buffer;
    sendHandler_ = handler;
}

}
}
}
}
}
}

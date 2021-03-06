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
namespace tcp
{

class socket: public BaseSocket
{
public:
    socket(io_service &ioService);

    void close();
    bool is_open() const;
    void shutdown(boost::asio::ip::tcp::socket::shutdown_type type);
    void async_receive(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler);
    void async_send(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler);
    void connect(const boost::asio::ip::tcp::endpoint &endpoint, boost::system::error_code& errorCode);
    void disconnect();

private:
    bool opened_;
};

}
}
}
}
}
}

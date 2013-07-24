#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <mt/env/asioStub/ioService.hpp>

namespace eMU {
namespace mt {
namespace env {
namespace asioStub {
namespace ip {
namespace tcp {

class socket {
public:
    typedef std::function<void(const boost::system::error_code&)> ConnectHandler;

    socket(io_service &service);
    void close();
    bool is_open() const;
    void shutdown(boost::asio::ip::tcp::socket::shutdown_type type);
    void async_receive(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler);
    void async_send(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler);
    void async_connect(const boost::asio::ip::tcp::endpoint &endpoint, const ConnectHandler &handler);

    io_service& get_io_service();
    io_service& service_;

    boost::asio::ip::tcp::endpoint remote_endpoint() const;
};

}
}
}
}
}
}

#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <ut/env/asioStub/ioService.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace asioStub {
namespace ip {
namespace tcp {

typedef boost::asio::ip::tcp::endpoint endpoint;

class socket {
public:
    typedef std::function<void(const boost::system::error_code&)> ConnectHandler;

    socket(io_service &service);

    MOCK_METHOD0(close, void());
    MOCK_METHOD0(is_open, bool());
    MOCK_METHOD1(shutdown, void(boost::asio::ip::tcp::socket::shutdown_type type));
    MOCK_METHOD2(async_receive, void(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler));
    MOCK_METHOD2(async_send, void(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler));
    MOCK_METHOD2(async_connect, void(const endpoint &endpoint, const ConnectHandler &handler));

    io_service& get_io_service();
    io_service& service_;

    endpoint remote_endpoint() const;
};

}
}
}
}
}
}

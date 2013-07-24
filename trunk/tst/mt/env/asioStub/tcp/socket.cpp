#include <mt/env/asioStub/tcp/socket.hpp>

namespace eMU {
namespace mt {
namespace env {
namespace asioStub {
namespace ip {
namespace tcp {

socket::socket(io_service &service):
  service_(service) {}

void socket::close() {

}

bool socket::is_open() const {
    return true;
}

void socket::shutdown(boost::asio::ip::tcp::socket::shutdown_type type) {

}

void socket::async_receive(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler) {

}

void socket::async_send(const boost::asio::mutable_buffers_1 &buffer, const io_service::IoHandler &handler) {

}

void socket::async_connect(const boost::asio::ip::tcp::endpoint &endpoint, const ConnectHandler &handler) {

}

io_service& socket::get_io_service() {
    return service_;
}

boost::asio::ip::tcp::endpoint socket::remote_endpoint() const {
    return boost::asio::ip::tcp::endpoint();
}

}
}
}
}
}
}

#include <mt/env/asioStub/udp/socket.hpp>

namespace eMU {
namespace mt {
namespace env {
namespace asioStub {
namespace ip {
namespace udp {

socket::socket(io_service &service, const boost::asio::ip::udp::endpoint &endpoint):
    service_(service),
    endpoint_(endpoint) {}

io_service& socket::get_io_service() {
    return service_;
}

void socket::async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                                boost::asio::ip::udp::endpoint &senderEndpoint,
                                const io_service::IoHandler &handler) {

}

void socket::async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                           boost::asio::ip::udp::endpoint endpoint,
                           const io_service::IoHandler &handler) {

}

}

}
}
}
}
}

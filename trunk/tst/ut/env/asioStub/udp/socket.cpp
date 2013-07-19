#include <ut/env/asioStub/udp/socket.hpp>

namespace eMU {
namespace ut {
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

}

}
}
}
}
}

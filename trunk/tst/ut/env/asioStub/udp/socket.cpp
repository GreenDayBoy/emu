#include <ut/env/asioStub/udp/socket.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace asioStub {
namespace ip {
namespace udp {

socket::socket(io_service &service, const endpoint &endpoint):
    service_(service),
    endpoint_(endpoint) {}

}
}
}
}
}
}

#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>

#include <ut/env/asioStub/ioService.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace asioStub {
namespace ip {
namespace udp {

typedef boost::asio::ip::udp::endpoint endpoint;

class socket {
public:
    socket(io_service &service, const endpoint &endpoint);

    MOCK_METHOD3(async_receive_from, void(const boost::asio::mutable_buffers_1 &buffer,
                                          endpoint &senderEndpoint,
                                          const io_service::IoHandler &handler));

    MOCK_METHOD3(async_send_to, void(const boost::asio::mutable_buffers_1 &buffer,
                                     endpoint endpoint,
                                     const io_service::IoHandler &handler));

    io_service service_;
    endpoint endpoint_;
};

}
}
}
}
}
}

#ifndef __ut_acceptorMock_hpp__
#define __ut_acceptorMock_hpp__

#include <gmock/gmock.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <env/core/ioServiceStub.hpp>
#include <env/core/tcp/tcpSocketMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

class acceptorMock_t: private boost::noncopyable {
public:
    typedef boost::function1<void, const boost::system::error_code&> acceptHandler_t;

    acceptorMock_t(ioServiceStub_t &ioService,
                   const boost::asio::ip::tcp::endpoint &endpoint);

    MOCK_METHOD2(async_accept, void(socketMock_t &socket, const acceptHandler_t &handler));
    void expectCall_async_accept();
    void impl_async_accept(socketMock_t &socket, const acceptHandler_t &handler);

    socketMock_t *socket_;
    acceptHandler_t acceptHandler_;

private:
    acceptorMock_t();
};

}
}
}
}
}

#endif
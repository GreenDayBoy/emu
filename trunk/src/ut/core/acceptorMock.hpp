#ifndef eMU_UT_ACCEPTORMOCK_HPP
#define eMU_UT_ACCEPTORMOCK_HPP

#include <boost/noncopyable.hpp>
#include <gmock/gmock.h>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "ioServiceStub.hpp"
#include "tcpSocketMock.hpp"

namespace eMU {
namespace ut {
namespace network {

class acceptorMock_t: private boost::noncopyable {
public:
    typedef boost::function1<void, const boost::system::error_code&> acceptHandler_t;

    acceptorMock_t(ioServiceStub_t &ioService,
                   const boost::asio::ip::tcp::endpoint &endpoint);

    MOCK_METHOD2(async_accept, void(tcp::socketMock_t &socket, const acceptHandler_t &handler));
    void expectCall_async_accept();
    void impl_async_accept(tcp::socketMock_t &socket, const acceptHandler_t &handler);

    acceptHandler_t acceptHandler_;
    tcp::socketMock_t *socket_;

private:
    acceptorMock_t();
};

}
}
}

#endif
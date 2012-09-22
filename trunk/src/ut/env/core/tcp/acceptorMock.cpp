#include <env/core/tcp/acceptorMock.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

acceptorMock_t::acceptorMock_t(ioServiceStub_t &ioService,
                               const boost::asio::ip::tcp::endpoint &endpoint) {
    ON_CALL(*this, async_accept(::testing::_,
                                ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                               &acceptorMock_t::impl_async_accept));
}

void acceptorMock_t::impl_async_accept(tcp::socketMock_t &socket, const acceptHandler_t &handler) {
    acceptHandler_ = handler;
}

void acceptorMock_t::expectCall_async_accept() {
    EXPECT_CALL(*this, async_accept(::testing::_, ::testing::_));
}

}
}
}
}
}
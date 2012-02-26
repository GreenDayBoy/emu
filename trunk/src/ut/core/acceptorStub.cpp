#include "acceptorStub.hpp"

namespace eMUNetworkUT = eMUUnitTest::networkTest;

void eMUNetworkUT::acceptorStub_t::async_accept(socketStub_t &socket, ioServiceMock_t::acceptHandler_t handler) {
    ioService_.accept(handler);
}

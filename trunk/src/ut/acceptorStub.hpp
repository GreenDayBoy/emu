#ifndef eMUUT_ACCEPTORSTUB_HPP
#define eMUUT_ACCEPTORSTUB_HPP

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include "../core/socket.hpp"
#include "ioServiceMock.hpp"
#include "socketStub.hpp"

namespace eMUUnitTest {
namespace networkTest {

namespace eMUNetwork = eMUCore::network;

class acceptorStub_t {
public:
    acceptorStub_t(ioServiceMock_t &ioService):
      ioService_(ioService) {}

    virtual ~acceptorStub_t() {}

    ioServiceMock_t& get_io_service() { return ioService_; }

    void async_accept(socketStub_t &socket, ioServiceMock_t::acceptHandler_t handler);


private:
    ioServiceMock_t &ioService_;
};

}
}

#endif
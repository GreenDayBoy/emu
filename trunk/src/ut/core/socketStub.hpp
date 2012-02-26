#ifndef eMUUT_SOCKETSTUB_HPP
#define eMUUT_SOCKETSTUB_HPP

#include <boost/asio.hpp>
#include "ioServiceMock.hpp"

namespace eMUUnitTest {
namespace networkTest {

class socketStub_t {
public:
    socketStub_t(ioServiceMock_t &ioService);
    virtual ~socketStub_t() {}

    void shutdown(boost::asio::ip::tcp::socket::shutdown_type type);
    void close();
    void async_receive(boost::asio::mutable_buffer &buf,
                       ioServiceMock_t::ioHandler_t handler);
    void async_send(boost::asio::mutable_buffer &buf,
                    ioServiceMock_t::ioHandler_t handler);
    bool is_open();

private:
    ioServiceMock_t &ioService_;
};

}
}

#endif
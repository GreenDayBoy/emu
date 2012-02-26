#include "socketStub.hpp"

namespace eMUNetworkUT = eMUUnitTest::networkTest;

eMUNetworkUT::socketStub_t::socketStub_t(ioServiceMock_t &ioService):
  ioService_(ioService) {}

void eMUNetworkUT::socketStub_t::shutdown(boost::asio::ip::tcp::socket::shutdown_type type) {
    ioService_.shutdownSocket(type);
}

void eMUNetworkUT::socketStub_t::close() {
    ioService_.closeSocket();
}

void eMUNetworkUT::socketStub_t::async_receive(boost::asio::mutable_buffer &buf,
                                               ioServiceMock_t::ioHandler_t handler) {
    ioService_.read(boost::asio::buffer_cast<uint8*>(buf),
                    boost::asio::buffer_size(buf),
                    handler);
}

void eMUNetworkUT::socketStub_t::async_send(boost::asio::mutable_buffer &buf,
                                            ioServiceMock_t::ioHandler_t handler) {
    ioService_.write(boost::asio::buffer_cast<const uint8*>(buf),
                     boost::asio::buffer_size(buf),
                     handler);
}

bool eMUNetworkUT::socketStub_t::is_open() {
    return true;
}
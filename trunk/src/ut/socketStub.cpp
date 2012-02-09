#include "socketStub.hpp"

eMUUnitTest::networkTest::socketStub_t::socketStub_t(ioServiceMock_t &ioService):
  ioService_(ioService) {}

void eMUUnitTest::networkTest::socketStub_t::shutdown(boost::asio::ip::tcp::socket::shutdown_type type) {
    ioService_.shutdownSocket(type);
}

void eMUUnitTest::networkTest::socketStub_t::close() {}

void eMUUnitTest::networkTest::socketStub_t::async_receive(boost::asio::mutable_buffer &buf,
                                                           ioServiceMock_t::ioHandler_t handler) {
    ioService_.read(boost::asio::buffer_cast<uint8*>(buf),
                    boost::asio::buffer_size(buf),
                    handler);
}

void eMUUnitTest::networkTest::socketStub_t::async_write(boost::asio::mutable_buffer &buf,
                                                         ioServiceMock_t::ioHandler_t handler) {
    ioService_.write(boost::asio::buffer_cast<const uint8*>(buf),
                     boost::asio::buffer_size(buf),
                     handler);
}
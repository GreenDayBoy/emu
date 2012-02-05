#include "socket.hpp"

eMUCore::network::socket_t::socket_t(boost::asio::ip::tcp::socket *socket):
  socket_(socket) {}

void eMUCore::network::socket_t::close() {
    socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket_->close();
}

void eMUCore::network::socket_t::queueRead(uint8 *payload, size_t size, ioHandler_t handler) {
    socket_->async_receive(boost::asio::buffer(payload, size), handler);
}

void eMUCore::network::socket_t::queueWrite(const uint8 *payload, size_t size, ioHandler_t handler) {
     boost::asio::async_write(*socket_,
                              boost::asio::buffer(payload, size),
                              handler);   
}
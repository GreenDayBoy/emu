#pragma once

#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include <mt/env/asioStub/types.hpp>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class io_service;

namespace ip
{
namespace tcp
{

class socket
{
public:
    socket(io_service &service);
    void close();
    bool is_open() const;
    void shutdown(boost::asio::ip::tcp::socket::shutdown_type type);
    void async_receive(const boost::asio::mutable_buffers_1 &buffer, const IoHandler &handler);
    void async_send(const boost::asio::mutable_buffers_1 &buffer, const IoHandler &handler);
    void async_connect(const boost::asio::ip::tcp::endpoint &endpoint, const ConnectHandler &handler);

    void insertPayload(const core::network::Payload &payload);
    core::network::Payload getPayload() const;

    io_service& get_io_service();

    boost::asio::ip::tcp::endpoint remote_endpoint() const;

private:
    io_service& service_;

    IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;

    IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;
};

}
}
}
}
}
}

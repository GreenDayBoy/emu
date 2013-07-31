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
namespace udp
{

class socket
{
public:
    socket(io_service &service, const boost::asio::ip::udp::endpoint &endpoint);

    void async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                            boost::asio::ip::udp::endpoint &senderEndpoint,
                            const IoHandler &handler);

    void async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                       boost::asio::ip::udp::endpoint endpoint,
                       const IoHandler &handler);

    io_service& get_io_service();

    void insertPayload(const core::network::Payload &payload);
    core::network::Payload getPayload() const;

private:
    io_service &service_;
    boost::asio::ip::udp::endpoint endpoint_;

    boost::asio::mutable_buffer receiveBuffer_;
    IoHandler receiveFromHandler_;

    boost::asio::mutable_buffer sendBuffer_;
    IoHandler sendToHandler_;
};

}
}
}
}
}
}

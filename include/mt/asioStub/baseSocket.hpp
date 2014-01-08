#pragma once

#include <boost/asio.hpp>
#include <mt/asioStub/ioService.hpp>
#include <core/network/payload.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class BaseSocket
{
public:
    BaseSocket(io_service &ioService);

    void send(const core::network::Payload &payload);
    core::network::Payload receive();
    io_service& get_io_service();
    bool isUnread() const;

protected:
    BaseSocket();

    io_service& ioService_;

    io_service::IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;

    io_service::IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;
};

}
}
}
}

#pragma once

#include <boost/asio.hpp>
#include <mt/env/asioStub/types.hpp>
#include <core/network/payload.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class io_service;

class BaseSocket
{
public:
    class ReceiveNotStartedException {};
    class SendNotStartedException {};
    class PayloadSizeOutOfBoundException {};

    BaseSocket(io_service &ioService);

    void insertPayload(const core::network::Payload &payload);
    core::network::Payload getPayload();
    io_service& get_io_service();
    bool hasUnreadPayload() const;

protected:
    BaseSocket();

    io_service& ioService_;

    IoHandler receiveHandler_;
    boost::asio::mutable_buffer receiveBuffer_;

    IoHandler sendHandler_;
    boost::asio::mutable_buffer sendBuffer_;
};

}
}
}
}

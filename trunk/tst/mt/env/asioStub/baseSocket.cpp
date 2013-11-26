#include <mt/env/asioStub/baseSocket.hpp>
#include <mt/env/asioStub/types.hpp>
#include <mt/env/asioStub/ioService.hpp>

#include <gtest/gtest.h>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

BaseSocket::BaseSocket(io_service &ioService):
    ioService_(ioService) {}

void BaseSocket::insertPayload(const core::network::Payload &payload)
{
    if(payload.getSize() > boost::asio::buffer_size(receiveBuffer_))
    {
        throw PayloadSizeOutOfBoundException();
    }

    uint8_t *buffer = boost::asio::buffer_cast<uint8_t*>(receiveBuffer_);

    if(buffer == nullptr)
    {
        throw ReceiveNotStartedException();
    }

    memcpy(buffer, &payload[0], payload.getSize());
    receiveBuffer_ = boost::asio::mutable_buffer();
    receiveHandler_(boost::system::error_code(), payload.getSize());
}

core::network::Payload BaseSocket::getPayload()
{
    const uint8_t *buffer = boost::asio::buffer_cast<const uint8_t*>(sendBuffer_);

    if(buffer == nullptr)
    {
        throw SendNotStartedException();
    }

    size_t size = boost::asio::buffer_size(sendBuffer_);

    if(size > core::network::Payload::getMaxSize())
    {
        throw PayloadSizeOutOfBoundException();
    }

    core::network::Payload payload;
    memcpy(&payload[0], buffer, size);
    payload.setSize(size);

    sendBuffer_ = boost::asio::mutable_buffer();
    sendHandler_(boost::system::error_code(), size);

    return payload;
}

io_service& BaseSocket::get_io_service()
{
    return ioService_;
}

bool BaseSocket::hasUnreadPayload() const
{
    return boost::asio::buffer_size(sendBuffer_) > 0;
}

}
}
}
}

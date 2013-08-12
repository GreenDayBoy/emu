#include <mt/env/asioStub/baseSocket.hpp>
#include <mt/env/asioStub/types.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/exceptions.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

BaseSocket::BaseSocket(io_service &ioService):
    ioService_(ioService)
{

}

void BaseSocket::insertPayload(const core::network::Payload &payload)
{
    if(payload.size() > boost::asio::buffer_size(receiveBuffer_))
    {
        throw exceptions::TooBigPayloadException();
    }

    uint8_t *buffer = boost::asio::buffer_cast<uint8_t*>(receiveBuffer_);

    if(buffer == nullptr)
    {
        throw exceptions::NullBufferException();
    }

    memcpy(buffer, &payload[0], payload.size());
    receiveBuffer_ = boost::asio::mutable_buffer();
    receiveHandler_(boost::system::error_code(), payload.size());
}

core::network::Payload BaseSocket::getPayload()
{
    const uint8_t *buffer = boost::asio::buffer_cast<const uint8_t*>(sendBuffer_);

    if(buffer == nullptr)
    {
        throw exceptions::NullBufferException();
    }

    size_t size = boost::asio::buffer_size(sendBuffer_);

    core::network::Payload payload(buffer, buffer + size);
    sendBuffer_ = boost::asio::mutable_buffer();
    sendHandler_(boost::system::error_code(), payload.size());

    return payload;
}

io_service& BaseSocket::get_io_service()
{
    return ioService_;
}

}
}
}
}

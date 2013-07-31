#include <mt/env/asioStub/udp/socket.hpp>
#include <mt/env/asioStub/exceptions.hpp>
#include <mt/env/asioStub/ioService.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{
namespace ip
{
namespace udp
{

socket::socket(io_service &service, const boost::asio::ip::udp::endpoint &endpoint):
    service_(service),
    endpoint_(endpoint) {}

io_service& socket::get_io_service()
{
    return service_;
}

void socket::async_receive_from(const boost::asio::mutable_buffers_1 &buffer,
                                boost::asio::ip::udp::endpoint &senderEndpoint,
                                const IoHandler &handler)
{
    receiveBuffer_ = buffer;
    receiveFromHandler_ = handler;

    service_.registerUdpSocket(this);
}

void socket::async_send_to(const boost::asio::mutable_buffers_1 &buffer,
                           boost::asio::ip::udp::endpoint endpoint,
                           const IoHandler &handler)
{
    sendBuffer_ = buffer;
    sendToHandler_ = handler;
}

void socket::insertPayload(const core::network::Payload &payload)
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
    receiveFromHandler_(boost::system::error_code(), payload.size());
}

core::network::Payload socket::getPayload() const
{
    const uint8_t *buffer = boost::asio::buffer_cast<const uint8_t*>(sendBuffer_);

    if(buffer == nullptr)
    {
        throw exceptions::NullBufferException();
    }

    size_t size = boost::asio::buffer_size(sendBuffer_);

    core::network::Payload payload(buffer, buffer + size);
    sendToHandler_(boost::system::error_code(), payload.size());

    return payload;
}

}

}
}
}
}
}

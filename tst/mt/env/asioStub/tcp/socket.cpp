#include <mt/env/asioStub/tcp/socket.hpp>
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/exceptions.hpp>
#include <glog/logging.h>

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
namespace tcp
{

socket::socket(io_service &service):
    service_(service) {}

void socket::close()
{
    service_.close(this);
}

bool socket::is_open() const
{
    return true;
}

void socket::shutdown(boost::asio::ip::tcp::socket::shutdown_type type) {}

void socket::async_receive(const boost::asio::mutable_buffers_1 &buffer, const IoHandler &handler)
{
    receiveBuffer_ = buffer;
    receiveHandler_ = handler;
}

void socket::async_send(const boost::asio::mutable_buffers_1 &buffer, const IoHandler &handler)
{
    sendBuffer_ = buffer;
    sendHandler_ = handler;
}

void socket::async_connect(const boost::asio::ip::tcp::endpoint &endpoint, const ConnectHandler &handler) {}

io_service& socket::get_io_service()
{
    return service_;
}

boost::asio::ip::tcp::endpoint socket::remote_endpoint() const
{
    return boost::asio::ip::tcp::endpoint();
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
    receiveHandler_(boost::system::error_code(), payload.size());
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
    sendHandler_(boost::system::error_code(), payload.size());

    return payload;
}

}
}
}
}
}
}

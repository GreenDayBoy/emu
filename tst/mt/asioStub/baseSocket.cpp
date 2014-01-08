#include <mt/asioStub/baseSocket.hpp>
#include <mt/asioStub/ioService.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

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

void BaseSocket::send(const core::network::Payload &payload)
{
    ASSERT_GT(boost::asio::buffer_size(receiveBuffer_), payload.getSize()) << "Sent payload size is out of bound!";

    uint8_t *buffer = boost::asio::buffer_cast<uint8_t*>(receiveBuffer_);

    ASSERT_THAT(buffer, ::testing::NotNull()) << "Receive not queued!";

    memcpy(buffer, &payload[0], payload.getSize());
    receiveBuffer_ = boost::asio::mutable_buffer();
    receiveHandler_(boost::system::error_code(), payload.getSize());
}

core::network::Payload BaseSocket::receive()
{
    const uint8_t *buffer = boost::asio::buffer_cast<const uint8_t*>(sendBuffer_);

    EXPECT_THAT(buffer, ::testing::NotNull()) << "Send not queued!";

    size_t size = boost::asio::buffer_size(sendBuffer_);

    EXPECT_GT(eMU::core::network::Payload::getMaxSize(), size) << "Received payload size is out of bound!";

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

bool BaseSocket::isUnread() const
{
    return boost::asio::buffer_size(sendBuffer_) > 0;
}

}
}
}
}

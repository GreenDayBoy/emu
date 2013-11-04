#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/network/udp/writeBufferFactory.hpp>

bool operator==(const eMU::core::network::WriteBuffer &left, const eMU::core::network::WriteBuffer &right)
{
    return left.payload_ == right.payload_ &&
           left.payloadSize_ == right.payloadSize_ &&
           left.secondPayload_ == right.secondPayload_&&
           left.secondPayloadSize_== right.secondPayloadSize_ &&
           left.pending_ == right.pending_;
}

TEST(writeBufferFactoryTest, ShouldReturnSameBufferForSameEndpoint)
{
    eMU::core::network::udp::WriteBufferFactory writeBufferFactory;

    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::WriteBuffer &writeBuffer = writeBufferFactory.get(endpoint);

    eMU::core::network::Payload payload(100, 0x10);
    writeBuffer.insert(payload);
    writeBuffer.pending_ = true;
    writeBuffer.insert(payload);

    eMU::core::network::WriteBuffer &writeBuffer2 = writeBufferFactory.get(endpoint);

    EXPECT_TRUE(writeBuffer == writeBuffer2);
}

TEST(writeBufferFactoryTest, ShouldRemoveBufferForGivenEndpoint)
{
    eMU::core::network::udp::WriteBufferFactory writeBufferFactory;

    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::WriteBuffer &writeBuffer = writeBufferFactory.get(endpoint);

    eMU::core::network::Payload payload(100, 0x10);
    writeBuffer.insert(payload);
    writeBuffer.pending_ = true;
    writeBuffer.insert(payload);

    ASSERT_TRUE(writeBufferFactory.erase(endpoint));

    eMU::core::network::WriteBuffer &writeBuffer2 = writeBufferFactory.get(endpoint);

    ASSERT_TRUE(eMU::core::network::WriteBuffer() == writeBuffer2);
}

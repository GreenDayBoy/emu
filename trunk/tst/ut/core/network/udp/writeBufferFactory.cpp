#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/network/udp/writeBufferFactory.hpp>
#include <ut/core/network/samplePayloads.hpp>

using eMU::ut::env::core::network::SamplePayloads;

using eMU::core::network::WriteBuffer;
using eMU::core::network::udp::WriteBufferFactory;

bool operator==(const WriteBuffer &left, const WriteBuffer &right)
{
    return left.getPayload().getSize() == right.getPayload().getSize() &&
           memcmp(&left.getPayload()[0], &right.getPayload()[0], left.getPayload().getSize()) == 0 &&
           left.getSecondPayload().getSize() == right.getSecondPayload().getSize() &&
           memcmp(&left.getSecondPayload()[0], &right.getSecondPayload()[0], left.getSecondPayload().getSize()) == 0 &&
           left.isPending() == right.isPending();
}

class WriteBufferFactoryTest: public ::testing::Test
{
protected:
    SamplePayloads samplePayloads_;
    WriteBufferFactory writeBufferFactory_;
};

TEST_F(WriteBufferFactoryTest, ShouldReturnSameBufferForSameEndpoint)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    writeBuffer.insert(samplePayloads_.payload1_);
    writeBuffer.setPendingState();
    writeBuffer.insert(samplePayloads_.payload2_);

    WriteBuffer &writeBuffer2 = writeBufferFactory_.get(endpoint);

    EXPECT_TRUE(writeBuffer == writeBuffer2);
}

TEST_F(WriteBufferFactoryTest, ShouldRemoveBufferForGivenEndpoint)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    writeBuffer.insert(samplePayloads_.payload1_);
    writeBuffer.setPendingState();
    writeBuffer.insert(samplePayloads_.payload2_);

    ASSERT_TRUE(writeBufferFactory_.erase(endpoint));

    WriteBuffer &writeBuffer2 = writeBufferFactory_.get(endpoint);

    ASSERT_TRUE(WriteBuffer() == writeBuffer2);
}

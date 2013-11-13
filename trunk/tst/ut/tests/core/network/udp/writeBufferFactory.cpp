#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/network/udp/writeBufferFactory.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

bool operator==(const eMU::core::network::WriteBuffer &left, const eMU::core::network::WriteBuffer &right)
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
    eMU::ut::env::core::network::SamplePayloads samplePayloads_;
    eMU::core::network::udp::WriteBufferFactory writeBufferFactory_;
};

TEST_F(WriteBufferFactoryTest, ShouldReturnSameBufferForSameEndpoint)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    writeBuffer.insert(samplePayloads_.payload1_);
    writeBuffer.setPendingState();
    writeBuffer.insert(samplePayloads_.payload2_);

    eMU::core::network::WriteBuffer &writeBuffer2 = writeBufferFactory_.get(endpoint);

    EXPECT_TRUE(writeBuffer == writeBuffer2);
}

TEST_F(WriteBufferFactoryTest, ShouldRemoveBufferForGivenEndpoint)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    writeBuffer.insert(samplePayloads_.payload1_);
    writeBuffer.setPendingState();
    writeBuffer.insert(samplePayloads_.payload2_);

    ASSERT_TRUE(writeBufferFactory_.erase(endpoint));

    eMU::core::network::WriteBuffer &writeBuffer2 = writeBufferFactory_.get(endpoint);

    ASSERT_TRUE(eMU::core::network::WriteBuffer() == writeBuffer2);
}

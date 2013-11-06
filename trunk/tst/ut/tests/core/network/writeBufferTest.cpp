#include <gtest/gtest.h>

#include <core/network/writeBuffer.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

class WriteBufferTest: public ::testing::Test
{
protected:
    eMU::ut::env::core::network::SamplePayloads samplePayloads_;
    eMU::core::network::WriteBuffer writeBuffer_;
};

TEST_F(WriteBufferTest, construct)
{
    EXPECT_FALSE(writeBuffer_.pending_);
}

TEST_F(WriteBufferTest, clear)
{
    writeBuffer_.payload_.setValue<uint8_t>(0, 0x01);
    writeBuffer_.secondPayload_.setValue<uint8_t>(0, 0x04);
    writeBuffer_.pending_ = true;

    writeBuffer_.clear();

    EXPECT_EQ(0, writeBuffer_.payload_.getSize());
    EXPECT_EQ(0, writeBuffer_.secondPayload_.getSize());

    EXPECT_FALSE(writeBuffer_.pending_);
}

TEST_F(WriteBufferTest, swap)
{
    writeBuffer_.secondPayload_ = samplePayloads_.payload1_;

    writeBuffer_.swap();

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), writeBuffer_.payload_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], &writeBuffer_.payload_[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(WriteBufferTest, insert)
{
    bool result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), writeBuffer_.payload_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], &writeBuffer_.payload_[0], samplePayloads_.fullFilledPayload_.getSize()), 0);
}

TEST_F(WriteBufferTest, insertBufferWhenPendingState)
{
    writeBuffer_.pending_ = true;

    bool result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), writeBuffer_.secondPayload_.getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], &writeBuffer_.secondPayload_[0], samplePayloads_.fullFilledPayload_.getSize()), 0);
}

TEST_F(WriteBufferTest, insertShouldReturnFalseWhenPrimaryBufferWillBeOverflow)
{
    writeBuffer_.pending_ = false;

    bool result = writeBuffer_.insert(samplePayloads_.fullFilledPayload_);

    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.payload1_);
    EXPECT_FALSE(result);
}

TEST_F(WriteBufferTest, insertShouldReturnFalseWhenSecondaryBufferWillBeOverflow)
{
    writeBuffer_.pending_ = true;

    bool result = writeBuffer_.insert(samplePayloads_.fullFilledPayload_);

    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.payload1_);
    EXPECT_FALSE(result);
}

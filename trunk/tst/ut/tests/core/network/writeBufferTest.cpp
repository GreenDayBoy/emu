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
    EXPECT_FALSE(writeBuffer_.isPending());
}

TEST_F(WriteBufferTest, clear)
{
    writeBuffer_.getPayload()[0] = 0x01;
    writeBuffer_.getSecondPayload()[0] = 0x04;
    writeBuffer_.setPendingState();

    writeBuffer_.clear();

    EXPECT_EQ(0, writeBuffer_.getPayload().getSize());
    EXPECT_EQ(0, writeBuffer_.getSecondPayload().getSize());

    EXPECT_FALSE(writeBuffer_.isPending());
}

TEST_F(WriteBufferTest, swap)
{
    writeBuffer_.getSecondPayload() = samplePayloads_.payload1_;

    writeBuffer_.swap();

    ASSERT_EQ(samplePayloads_.payload1_.getSize(), writeBuffer_.getPayload().getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.payload1_[0], &writeBuffer_.getPayload()[0], samplePayloads_.payload1_.getSize()), 0);
}

TEST_F(WriteBufferTest, insert)
{
    bool result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), writeBuffer_.getPayload().getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], &writeBuffer_.getPayload()[0], samplePayloads_.fullFilledPayload_.getSize()), 0);
}

TEST_F(WriteBufferTest, insertBufferWhenPendingState)
{
    writeBuffer_.setPendingState();

    bool result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.halfFilledPayload_);
    ASSERT_TRUE(result);

    ASSERT_EQ(samplePayloads_.fullFilledPayload_.getSize(), writeBuffer_.getSecondPayload().getSize());
    ASSERT_EQ(memcmp(&samplePayloads_.fullFilledPayload_[0], &writeBuffer_.getSecondPayload()[0], samplePayloads_.fullFilledPayload_.getSize()), 0);
}

TEST_F(WriteBufferTest, insertShouldReturnFalseWhenPrimaryBufferWillBeOverflow)
{
    writeBuffer_.clearPendingState();

    bool result = writeBuffer_.insert(samplePayloads_.fullFilledPayload_);

    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.payload1_);
    EXPECT_FALSE(result);
}

TEST_F(WriteBufferTest, insertShouldReturnFalseWhenSecondaryBufferWillBeOverflow)
{
    writeBuffer_.setPendingState();

    bool result = writeBuffer_.insert(samplePayloads_.fullFilledPayload_);

    ASSERT_TRUE(result);

    result = writeBuffer_.insert(samplePayloads_.payload1_);
    EXPECT_FALSE(result);
}

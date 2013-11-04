#include <gtest/gtest.h>

#include <core/network/buffer.hpp>

TEST(readBufferTest, construct)
{
    eMU::core::network::ReadBuffer readBuffer;

    EXPECT_EQ(eMU::core::network::kMaxPayloadSize, readBuffer.payload_.size());
    EXPECT_EQ(0u, readBuffer.payloadSize_);
}

TEST(readBufferTest, clear)
{
    eMU::core::network::ReadBuffer readBuffer;
    readBuffer.payload_[0] = 0x01;
    readBuffer.payload_[eMU::core::network::kMaxPayloadSize - 1] = 0x02;
    readBuffer.payloadSize_ = eMU::core::network::kMaxPayloadSize;

    readBuffer.clear();

    ASSERT_EQ(eMU::core::network::kMaxPayloadSize, readBuffer.payload_.size());
    EXPECT_EQ(0x00, readBuffer.payload_[0]);
    EXPECT_EQ(0x00, readBuffer.payload_[eMU::core::network::kMaxPayloadSize - 1]);
    EXPECT_EQ(0u, readBuffer.payloadSize_);
}

class WriteBufferTest: public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        this->initializePatternBuffer();
    }

    void initializePatternBuffer()
    {
        uint8_t value = 0;

        for(auto &field : patternBuffer_.payload_)
        {
            field = value++;
        }

        patternBuffer_.payloadSize_ = eMU::core::network::kMaxPayloadSize;
    }

    eMU::core::network::WriteBuffer patternBuffer_;
};

TEST_F(WriteBufferTest, construct)
{
    eMU::core::network::WriteBuffer writeBuffer;

    EXPECT_FALSE(writeBuffer.pending_);

    EXPECT_EQ(eMU::core::network::kMaxPayloadSize, writeBuffer.payload_.size());
    EXPECT_EQ(0u, writeBuffer.payloadSize_);

    EXPECT_EQ(eMU::core::network::kMaxPayloadSize, writeBuffer.secondPayload_.size());
    EXPECT_EQ(0u, writeBuffer.secondPayloadSize_);
}

TEST_F(WriteBufferTest, clear)
{
    eMU::core::network::WriteBuffer writeBuffer;
    writeBuffer.payload_[0] = 0x01;
    writeBuffer.payload_[eMU::core::network::kMaxPayloadSize - 1] = 0x02;
    writeBuffer.payloadSize_ = eMU::core::network::kMaxPayloadSize;

    writeBuffer.secondPayload_[0] = 0x03;
    writeBuffer.secondPayload_[eMU::core::network::kMaxPayloadSize - 1] = 0x04;
    writeBuffer.secondPayloadSize_ = eMU::core::network::kMaxPayloadSize;

    writeBuffer.pending_ = true;

    writeBuffer.clear();

    ASSERT_EQ(eMU::core::network::kMaxPayloadSize, writeBuffer.payload_.size());
    EXPECT_EQ(0x00, writeBuffer.payload_[0]);
    EXPECT_EQ(0x00, writeBuffer.payload_[eMU::core::network::kMaxPayloadSize - 1]);
    EXPECT_EQ(0u, writeBuffer.payloadSize_);

    ASSERT_EQ(eMU::core::network::kMaxPayloadSize, writeBuffer.secondPayload_.size());
    EXPECT_EQ(0x00, writeBuffer.secondPayload_[0]);
    EXPECT_EQ(0x00, writeBuffer.secondPayload_[eMU::core::network::kMaxPayloadSize - 1]);
    EXPECT_EQ(0u, writeBuffer.secondPayloadSize_);

    EXPECT_FALSE(writeBuffer.pending_);
}

TEST_F(WriteBufferTest, swap)
{
    eMU::core::network::WriteBuffer writeBuffer;
    writeBuffer.secondPayload_ = patternBuffer_.payload_;
    writeBuffer.secondPayloadSize_ = patternBuffer_.payloadSize_;

    writeBuffer.swap();

    EXPECT_EQ(patternBuffer_.payload_, writeBuffer.payload_);
    EXPECT_EQ(patternBuffer_.payloadSize_, writeBuffer.payloadSize_);
}

TEST_F(WriteBufferTest, insert)
{
    eMU::core::network::WriteBuffer writeBuffer;

    size_t half_payload = eMU::core::network::kMaxPayloadSize / 2;
    eMU::core::network::Payload first_payload(patternBuffer_.payload_.begin(), patternBuffer_.payload_.begin() + half_payload);

    bool result = writeBuffer.insert(first_payload);
    ASSERT_TRUE(result);

    eMU::core::network::Payload second_payload(patternBuffer_.payload_.begin() + half_payload, patternBuffer_.payload_.end());
    result = writeBuffer.insert(second_payload);
    ASSERT_TRUE(result);

    EXPECT_EQ(patternBuffer_.payload_, writeBuffer.payload_);
    EXPECT_EQ(patternBuffer_.payloadSize_, writeBuffer.payloadSize_);
}

TEST_F(WriteBufferTest, insertBufferWhenPendingState)
{
    eMU::core::network::WriteBuffer writeBuffer;

    writeBuffer.pending_ = true;

    size_t half_payload = eMU::core::network::kMaxPayloadSize / 2;
    eMU::core::network::Payload firstPayload(patternBuffer_.payload_.begin(), patternBuffer_.payload_.begin() + half_payload);
    bool result = writeBuffer.insert(firstPayload);
    ASSERT_TRUE(result);

    eMU::core::network::Payload secondPayload(patternBuffer_.payload_.begin() + half_payload, patternBuffer_.payload_.end());
    result = writeBuffer.insert(secondPayload);
    ASSERT_TRUE(result);

    EXPECT_EQ(patternBuffer_.payload_, writeBuffer.secondPayload_);
    EXPECT_EQ(patternBuffer_.payloadSize_, writeBuffer.secondPayloadSize_);
}

TEST_F(WriteBufferTest, insert_OverflowPrimaryBuffer)
{
    eMU::core::network::WriteBuffer writeBuffer;
    writeBuffer.pending_ = false;

    bool result = writeBuffer.insert(patternBuffer_.payload_);

    ASSERT_TRUE(result);
    EXPECT_EQ(patternBuffer_.payload_, writeBuffer.payload_);
    EXPECT_EQ(patternBuffer_.payloadSize_, writeBuffer.payloadSize_);

    result = writeBuffer.insert(patternBuffer_.payload_);
    EXPECT_FALSE(result);
}

TEST_F(WriteBufferTest, insert_overflowSecondaryBuffer)
{
    eMU::core::network::WriteBuffer writeBuffer;
    writeBuffer.pending_ = true;
    bool result = writeBuffer.insert(patternBuffer_.payload_);

    ASSERT_TRUE(result);
    EXPECT_EQ(patternBuffer_.payload_, writeBuffer.secondPayload_);
    EXPECT_EQ(patternBuffer_.payloadSize_, writeBuffer.secondPayloadSize_);

    result = writeBuffer.insert(patternBuffer_.payload_);
    EXPECT_FALSE(result);
}

#include <gtest/gtest.h>
#include "../core/buffer.hpp"

namespace eMUNetwork = eMUCore::network;

TEST(readBufferTest, construct) {
    eMUNetwork::readBuffer_t rbuf;

    EXPECT_EQ(eMUNetwork::maxPayloadSize_c, rbuf.payload_.size());
    EXPECT_EQ(0, rbuf.payloadSize_);
}

TEST(readBufferTest, clear) {
    eMUNetwork::readBuffer_t rbuf;
    rbuf.payload_[0] = 0x00;
    rbuf.payload_[eMUNetwork::maxPayloadSize_c - 1] = 0x01;

    rbuf.clear();

    ASSERT_EQ(eMUNetwork::maxPayloadSize_c, rbuf.payload_.size());
    EXPECT_EQ(0x00, rbuf.payload_[0]);
    EXPECT_EQ(0x00, rbuf.payload_[eMUNetwork::maxPayloadSize_c - 1]);
}

class writeBufferTest_t: public ::testing::Test {
protected:
    virtual void SetUp() {
        initializePatternBuf();
    }

    void initializePatternBuf() {
        uint8 value = 0;
        for(size_t i = 0; i < eMUNetwork::maxPayloadSize_c; ++i)
            patternBuf_.payload_[i] = value++;
        
        patternBuf_.payloadSize_ = eMUNetwork::maxPayloadSize_c;
    }

    eMUNetwork::writeBuffer_t patternBuf_;
};

TEST_F(writeBufferTest_t, construct) {
    eMUNetwork::writeBuffer_t wbuf;

    EXPECT_FALSE(wbuf.pending_);
    EXPECT_EQ(eMUNetwork::maxPayloadSize_c, wbuf.payload_.size());
    EXPECT_EQ(0, wbuf.payloadSize_);
    EXPECT_EQ(eMUNetwork::maxPayloadSize_c, wbuf.secPayload_.size());
    EXPECT_EQ(0, wbuf.secPayloadSize_);
}

TEST_F(writeBufferTest_t, clear) {
    eMUNetwork::writeBuffer_t wbuf;
    wbuf.payload_[0] = 0x00;
    wbuf.payload_[eMUNetwork::maxPayloadSize_c - 1] = 0x01;
    wbuf.payloadSize_ = 2;

    wbuf.secPayload_[0] = 0x02;
    wbuf.secPayload_[eMUNetwork::maxPayloadSize_c - 1] = 0x03;
    wbuf.secPayloadSize_ = 2;

    wbuf.pending_ = true;

    wbuf.clear();

    ASSERT_EQ(eMUNetwork::maxPayloadSize_c, wbuf.payload_.size());
    EXPECT_EQ(0x00, wbuf.payload_[0]);
    EXPECT_EQ(0x00, wbuf.payload_[eMUNetwork::maxPayloadSize_c - 1]);
    EXPECT_EQ(0, wbuf.payloadSize_);

    ASSERT_EQ(eMUNetwork::maxPayloadSize_c, wbuf.secPayload_.size());
    EXPECT_EQ(0x00, wbuf.secPayload_[0]);
    EXPECT_EQ(0x00, wbuf.secPayload_[eMUNetwork::maxPayloadSize_c - 1]);
    EXPECT_EQ(0, wbuf.secPayloadSize_);

    EXPECT_FALSE(wbuf.pending_);
}

TEST_F(writeBufferTest_t, swap) {
    eMUNetwork::writeBuffer_t wbuf;
    wbuf.secPayload_ = patternBuf_.payload_;
    wbuf.secPayloadSize_ = patternBuf_.payloadSize_;

    wbuf.swap();

    EXPECT_EQ(patternBuf_.payload_, wbuf.payload_);
    EXPECT_EQ(patternBuf_.payloadSize_, wbuf.payloadSize_);
}

TEST_F(writeBufferTest_t, insert_PendingState) {
    size_t partPayloadSize = patternBuf_.payloadSize_ / 2;
    eMUNetwork::writeBuffer_t wbuf;
    wbuf.pending_ = true;

    bool result = wbuf.insert(&patternBuf_.payload_[0], partPayloadSize);
    ASSERT_TRUE(result);

    result = wbuf.insert(&patternBuf_.payload_[partPayloadSize], partPayloadSize);
    ASSERT_TRUE(result);

    EXPECT_EQ(patternBuf_.payload_, wbuf.secPayload_);
    EXPECT_EQ(patternBuf_.payloadSize_, wbuf.secPayloadSize_);

    wbuf.pending_ = false;

    result = wbuf.insert(&patternBuf_.payload_[0], partPayloadSize);
    ASSERT_TRUE(result);

    result = wbuf.insert(&patternBuf_.payload_[partPayloadSize], partPayloadSize);
    ASSERT_TRUE(result);

    EXPECT_EQ(patternBuf_.payload_, wbuf.payload_);
    EXPECT_EQ(patternBuf_.payloadSize_, wbuf.payloadSize_);
}

TEST_F(writeBufferTest_t, insert_OverflowPrimaryBuffer) {
    eMUNetwork::writeBuffer_t wbuf;
    wbuf.pending_ = false;
    bool result = wbuf.insert(&patternBuf_.payload_[0], patternBuf_.payloadSize_);

    ASSERT_TRUE(result);
    EXPECT_EQ(patternBuf_.payload_, wbuf.payload_);
    EXPECT_EQ(patternBuf_.payloadSize_, wbuf.payloadSize_);

    result = wbuf.insert(&patternBuf_.payload_[0], patternBuf_.payloadSize_);
    EXPECT_FALSE(result);
}

TEST_F(writeBufferTest_t, insert_OverflowSecondaryBuffer) {
    eMUNetwork::writeBuffer_t wbuf;
    wbuf.pending_ = true;
    bool result = wbuf.insert(&patternBuf_.payload_[0], patternBuf_.payloadSize_);

    ASSERT_TRUE(result);
    EXPECT_EQ(patternBuf_.payload_, wbuf.secPayload_);
    EXPECT_EQ(patternBuf_.payloadSize_, wbuf.secPayloadSize_);

    result = wbuf.insert(&patternBuf_.payload_[0], patternBuf_.payloadSize_);
    EXPECT_FALSE(result);
}
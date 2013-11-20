#include <protocol/readStreamsExtractor.hpp>

#include <gtest/gtest.h>

namespace network = eMU::core::network;
namespace protocol = eMU::protocol;

class ReadStreamsExtractorTest: public ::testing::Test
{
public:
    ReadStreamsExtractorTest():
        extractor_(payload_) {}

protected:
    network::Payload payload_;
    protocol::ReadStreamsExtractor extractor_;
};

TEST_F(ReadStreamsExtractorTest, extract)
{
    const size_t stream1Size = 8;
    reinterpret_cast<uint32_t&>(payload_[0]) = stream1Size;
    reinterpret_cast<uint16_t&>(payload_[4]) = 0xFFFF;
    reinterpret_cast<uint16_t&>(payload_[6]) = 0xEEEE;
    reinterpret_cast<uint32_t&>(payload_[8]) = 0xDDDDDDDD;

    const size_t stream2Size = 4;
    reinterpret_cast<uint32_t&>(payload_[12]) = stream2Size;
    reinterpret_cast<uint16_t&>(payload_[16]) = 0xCCCC;
    reinterpret_cast<uint16_t&>(payload_[18]) = 0xBBBB;

    const size_t stream3Size = 6;
    reinterpret_cast<uint32_t&>(payload_[20]) = stream3Size;
    reinterpret_cast<uint16_t&>(payload_[24]) = 0xAAAA;
    reinterpret_cast<uint32_t&>(payload_[26]) = 0xFFFFFFFF;

    payload_.setSize(30);

    extractor_.extract();

    protocol::ReadStreamsExtractor::StreamsContainer streams = extractor_.getStreams();
    ASSERT_EQ(3, streams.size());

    ASSERT_EQ(0xFFFF, streams[0].getId());
    ASSERT_EQ(0xEEEE, streams[0].readNext<uint16_t>());
    ASSERT_EQ(0xDDDDDDDD, streams[0].readNext<uint32_t>());

    ASSERT_EQ(0xCCCC, streams[1].getId());
    ASSERT_EQ(0xBBBB, streams[1].readNext<uint16_t>());

    ASSERT_EQ(0xAAAA, streams[2].getId());
    ASSERT_EQ(0xFFFFFFFF, streams[2].readNext<uint32_t>());
}

TEST_F(ReadStreamsExtractorTest, EmptyPayloadShouldThrowException)
{
    bool exceptionThrown = false;

    try
    {
        extractor_.extract();
    }
    catch(const protocol::ReadStreamsExtractor::EmptyPayloadException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(ReadStreamsExtractorTest, StreamWithTooLargeSizeShouldThrowException)
{
    reinterpret_cast<uint32_t&>(payload_[0]) = 5;
    payload_.setSize(4);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();
    }
    catch(const protocol::ReadStreamsExtractor::UnknownStreamFormatException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(ReadStreamsExtractorTest, StreamWithZeroSizeShouldThrowException)
{
    reinterpret_cast<uint32_t&>(payload_[0]) = 0;
    payload_.setSize(20);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();

    }
    catch(const protocol::ReadStreamsExtractor::EmptyStreamException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

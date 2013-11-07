#include <core/protocol/packetsExtractor.hpp>

#include <gtest/gtest.h>

namespace network = eMU::core::network;
namespace protocol = eMU::core::protocol;

class PacketsExtractorTest: public ::testing::Test
{
public:
    PacketsExtractorTest():
        extractor_(payload_) {}

protected:
    network::Payload payload_;
    protocol::PacketsExtractor extractor_;
};

TEST_F(PacketsExtractorTest, extract)
{
    const size_t packet1Size = 10;
    payload_.insert<uint32_t>(packet1Size);
    payload_.insert<uint16_t>(0xFFFF);
    payload_.insert<uint32_t>(0xFFFFDDDD);
    payload_.insert<uint32_t>(0xBBBBCCCC);

    const size_t packet2Size = 4;
    payload_.insert<uint32_t>(packet2Size);
    payload_.insert<uint16_t>(0xABCD);
    payload_.insert<uint16_t>(0xEFAB);

    const size_t packet3Size = 12;
    payload_.insert<uint32_t>(packet3Size);
    payload_.insert<uint32_t>(0xAAAABBBB);
    payload_.insert<uint32_t>(0xCCCCDDDD);
    payload_.insert<uint32_t>(0xEEEEFFFF);

    extractor_.extract();

    const protocol::PacketsExtractor::PacketsContainer& packets = extractor_.getPackets();
    ASSERT_EQ(3, packets.size());

    const size_t packetHeaderSize = sizeof(uint32_t);

    ASSERT_EQ(packet1Size, packets[0].getSize());
    size_t offset = packetHeaderSize;
    ASSERT_EQ(memcmp(&payload_[offset], &packets[0][0], packet1Size), 0);

    ASSERT_EQ(packet2Size, packets[1].getSize());
    offset += packet1Size + packetHeaderSize;
    ASSERT_EQ(memcmp(&payload_[offset], &packets[1][0], packet2Size), 0);

    ASSERT_EQ(packet3Size, packets[2].getSize());
    offset += packet2Size + packetHeaderSize;
    ASSERT_EQ(memcmp(&payload_[offset], &packets[2][0], packet3Size), 0);
}

TEST_F(PacketsExtractorTest, gotEmptyPayloadShouldThrowException)
{
    bool exceptionThrown = false;

    try
    {
        extractor_.extract();
    }
    catch(const protocol::PacketsExtractor::EmptyPayloadException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PacketsExtractorTest, PacketWithOutOfBoundSizeShouldThrowException)
{
    payload_.insert<uint32_t>(network::Payload::getMaxSize() + 1);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();
    }
    catch(const protocol::PacketsExtractor::PacketSizeOutOfBoundException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PacketsExtractorTest, WhenPacketSizeIsEqualToZeroShouldThrowException)
{
    payload_.insert<uint32_t>(0);
    payload_.insert<uint32_t>(0xAAAABBBB);
    payload_.insert<uint32_t>(0xCCCCDDDD);
    payload_.insert<uint32_t>(0xEEEEFFFF);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();

    }
    catch(const protocol::PacketsExtractor::NullPacketSizeException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PacketsExtractorTest, TooSmallPacketSizeShouldThrowException)
{
    payload_.insert<uint32_t>(4);
    payload_.insert<uint32_t>(0xAAAABBBB);
    payload_.insert<uint32_t>(2);
    payload_.insert<uint16_t>(0xEEEE);
    payload_.insert<uint32_t>(1);
    payload_.insert<uint16_t>(0xABCD);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();

    }
    catch(const protocol::PacketsExtractor::PacketSizeOutOfBoundException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PacketsExtractorTest, TooLargePacketSizeShouldThrowException)
{
    payload_.insert<uint32_t>(4);
    payload_.insert<uint32_t>(0xAAAABBBB);
    payload_.insert<uint32_t>(2);
    payload_.insert<uint16_t>(0xEEEE);
    payload_.insert<uint32_t>(4);
    payload_.insert<uint16_t>(0xABCD);

    bool exceptionThrown = false;

    try
    {
        extractor_.extract();

    }
    catch(const protocol::PacketsExtractor::PacketSizeOutOfBoundException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

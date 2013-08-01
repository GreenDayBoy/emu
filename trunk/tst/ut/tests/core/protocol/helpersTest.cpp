#include <gtest/gtest.h>

#include <core/network/buffer.hpp>
#include <core/protocol/helpers.hpp>

namespace network = eMU::core::network;
namespace protocol = eMU::core::protocol;

TEST(HelperTest, byteSwap)
{
    EXPECT_EQ(0xFFFF, protocol::byteSwap(0xFFFF));
    EXPECT_EQ(0x30FF, protocol::byteSwap(0xFF30));
    EXPECT_EQ(0x0050, protocol::byteSwap(0x5000));
    EXPECT_EQ(0x4600, protocol::byteSwap(0x0046));
    EXPECT_EQ(0x0000, protocol::byteSwap(0x0000));
}

TEST(HelperTest, getersForSmallMessage)
{
    network::Payload payload = {0xC1, 0x30, 0xF4};
    EXPECT_EQ(payload[2], protocol::getProtocolId(payload));
    EXPECT_EQ(payload[1], protocol::getSize(payload));

    payload[0] = 0xC3;
    EXPECT_EQ(payload[2], protocol::getProtocolId(payload));
    EXPECT_EQ(payload[1], protocol::getSize(payload));
}

TEST(HelperTest, getersForLargeMessage)
{
    network::Payload payload = {0xC2, 0x30, 0xFF, 0xF4};
    EXPECT_EQ(payload[3], protocol::getProtocolId(payload));
    EXPECT_EQ(0xFF30, protocol::getSize(payload));

    payload[0] = 0xC4;
    EXPECT_EQ(payload[3], protocol::getProtocolId(payload));
    EXPECT_EQ(0xFF30, protocol::getSize(payload));
}

TEST(HelperTest, getersForInvalidMessage)
{
    network::Payload payload = {0xFF, 0x30, 0xFF, 0xF4};
    EXPECT_EQ(0, protocol::getProtocolId(payload));
    EXPECT_EQ(0, protocol::getSize(payload));
}

TEST(HelperTest, isCrypted)
{
    network::Payload payload = {0xC3, 0x04, 0x00, 0x00};
    EXPECT_TRUE(protocol::isCrypted(payload));

    payload[0] = 0xC4;
    EXPECT_TRUE(protocol::isCrypted(payload));

    payload[0] = 0xC1;
    EXPECT_FALSE(protocol::isCrypted(payload));

    payload[0] = 0xC2;
    EXPECT_FALSE(protocol::isCrypted(payload));
}

TEST(HelperTest, isValid)
{
    network::Payload payload = {0xC1, 0x04, 0x00, 0x00};
    EXPECT_TRUE(protocol::hasValidHeader(payload));

    payload[0] = 0xC2;
    EXPECT_TRUE(protocol::hasValidHeader(payload));

    payload[0] = 0xC3;
    EXPECT_TRUE(protocol::hasValidHeader(payload));

    payload[0] = 0xC4;
    EXPECT_TRUE(protocol::hasValidHeader(payload));

    payload[0] = 0xC5;
    EXPECT_FALSE(protocol::hasValidHeader(payload));
}

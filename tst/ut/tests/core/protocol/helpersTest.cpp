#include <gtest/gtest.h>

#include <core/network/protocol/helpers.hpp>

TEST(HelperTest, byteSwap)
{
    EXPECT_EQ(0xFFFF, eMU::core::network::protocol::byteSwap(0xFFFF));
    EXPECT_EQ(0x30FF, eMU::core::network::protocol::byteSwap(0xFF30));
    EXPECT_EQ(0x0050, eMU::core::network::protocol::byteSwap(0x5000));
    EXPECT_EQ(0x4600, eMU::core::network::protocol::byteSwap(0x0046));
    EXPECT_EQ(0x0000, eMU::core::network::protocol::byteSwap(0x0000));
}

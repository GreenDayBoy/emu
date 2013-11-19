#include <protocol/writeStream.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>

namespace protocol = eMU::protocol;

class WriteStreamTest: public ::testing::Test
{
public:
    WriteStreamTest() {}
};

TEST_F(WriteStreamTest, asd)
{
    protocol::WriteStream writeStream(0xAABB);

    writeStream.write<uint16_t>(0xCCDD);
    writeStream.write<uint32_t>(0xEEFFAABB);

    ASSERT_EQ(writeStream.getSize(), 8);
}

#include <protocol/writeStream.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>

using eMU::protocol::WriteStream;
using eMU::core::network::Payload;

class WriteStreamTest: public ::testing::Test
{
protected:
    WriteStreamTest() {}
};

TEST_F(WriteStreamTest, writeNext)
{
    uint16_t id = 0xAABB;
    std::string testString = "writeNextTest";
    uint32_t testStringLength = testString.length();
    size_t payloadSize = sizeof(id) + sizeof(testStringLength) + testString.length();

    Payload payload;
    reinterpret_cast<uint32_t&>(payload[0]) = payloadSize;
    reinterpret_cast<uint16_t&>(payload[4]) = id;
    reinterpret_cast<uint32_t&>(payload[6]) = testStringLength;

    payload.setSize((payloadSize + sizeof(uint32_t)));

    for(size_t i = 0; i < testString.length(); ++i)
    {
        payload[10 + i] = testString[i];
    }

    WriteStream writeStream(id);
    writeStream.writeNext<uint32_t>(testStringLength);
    writeStream.writeNext(testString);

    ASSERT_EQ(payload.getSize(), writeStream.getPayload().getSize());

    int32_t result = memcmp(&payload[0], &writeStream.getPayload()[0], payload.getSize());
    ASSERT_EQ(0, result);
}

TEST_F(WriteStreamTest, writeNextShouldThrowExceptionWhenOffsetIsOutOfBound)
{
   WriteStream writeStream(0xFFFF);

   std::string value(Payload::getMaxSize(), 'A');

   ASSERT_THROW(writeStream.writeNext(value), WriteStream::OverflowException);
}

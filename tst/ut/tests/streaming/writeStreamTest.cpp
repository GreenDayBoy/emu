#include <streaming/writeStream.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>

using eMU::streaming::WriteStream;
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
    writeStream.writeNextString(testString);

    ASSERT_EQ(payload.getSize(), writeStream.getPayload().getSize());

    int32_t result = memcmp(&payload[0], &writeStream.getPayload()[0], payload.getSize());
    ASSERT_EQ(0, result);
}

TEST_F(WriteStreamTest, writeNextShouldThrowExceptionWhenOffsetIsOutOfBound)
{
   WriteStream writeStream(0xFFFF);

   std::string value(Payload::getMaxSize(), 'A');

   ASSERT_THROW(writeStream.writeNextString(value), WriteStream::OverflowException);
}

TEST_F(WriteStreamTest, writeNextWideString)
{
   WriteStream writeStream(0xFFFF);

   std::wstring expectedValue = L"Another perfect wide string";
   writeStream.writeNextWideString(expectedValue);

   size_t dataOffset = 6; // sizeof(uint32) -> size + sizeof(uint16_t) -> id

   std::wstring value;
   for(size_t i = 0; i < expectedValue.length(); ++i)
   {
       size_t offset = i * sizeof(char16_t);
       value.push_back(reinterpret_cast<const char16_t&>(writeStream.getPayload()[dataOffset + offset]));
   }

   ASSERT_EQ(expectedValue, value);
}


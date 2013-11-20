#include <protocol/writeStream.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>

namespace protocol = eMU::protocol;

class WriteStreamTest: public ::testing::Test
{
public:
    WriteStreamTest() {}
};

TEST_F(WriteStreamTest, writeNext)
{
    uint16_t id = 0xAABB;
    std::string testString = "writeNextTest";
    uint32_t testStringLength = testString.length();
    size_t payloadSize = sizeof(id) + sizeof(testStringLength) + testString.length();

    eMU::core::network::Payload payload;
    reinterpret_cast<uint32_t&>(payload[0]) = payloadSize;
    reinterpret_cast<uint16_t&>(payload[4]) = id;
    reinterpret_cast<uint32_t&>(payload[6]) = testStringLength;

    payload.setSize((payloadSize + sizeof(uint32_t)));

    for(size_t i = 0; i < testString.length(); ++i)
    {
        payload[10 + i] = testString[i];
    }

    protocol::WriteStream writeStream(id);
    writeStream.writeNext<uint32_t>(testStringLength);
    writeStream.writeNext(testString);

    ASSERT_EQ(payload.getSize(), writeStream.getPayload().getSize());

    int32_t result = memcmp(&payload[0], &writeStream.getPayload()[0], payload.getSize());
    ASSERT_EQ(0, result);
}

TEST_F(WriteStreamTest, writeNextShouldThrowExceptionWhenOffsetIsOutOfBound)
{
   protocol::WriteStream writeStream(0xFFFF);

   bool exceptionThrown = false;

   std::string value(eMU::core::network::Payload::getMaxSize(), 'A');

   try
   {
       writeStream.writeNext(value);
   }
   catch(const protocol::WriteStream::OverflowException&)
   {
       exceptionThrown = true;
   }

   ASSERT_TRUE(exceptionThrown);
}

#include <protocol/readStream.hpp>
#include <ut/env/core/network/samplePayloads.hpp>

#include <gtest/gtest.h>
#include <boost/locale.hpp>

using eMU::protocol::ReadStream;
using eMU::core::network::Payload;
using eMU::ut::env::core::network::SamplePayloads;

class ReadStreamTest: public ::testing::Test
{
protected:
    ReadStreamTest():
        readStream_(samplePayloads_.fullFilledPayload_) {}

    SamplePayloads samplePayloads_;
    ReadStream readStream_;
};

TEST_F(ReadStreamTest, readNextStringShouldThrowExceptionWhenStringLengthIsOutOfBound)
{
    ASSERT_THROW(readStream_.readNextString(samplePayloads_.fullFilledPayload_.getSize()), ReadStream::OverflowException);
}

TEST_F(ReadStreamTest, readNext)
{
    uint32_t &value1 = reinterpret_cast<uint32_t&>(samplePayloads_.fullFilledPayload_[6]);
    ASSERT_EQ(value1, readStream_.readNext<uint32_t>());

    uint16_t &value2 = reinterpret_cast<uint16_t&>(samplePayloads_.fullFilledPayload_[10]);
    ASSERT_EQ(value2, readStream_.readNext<uint16_t>());

    uint8_t value3 = samplePayloads_.fullFilledPayload_[12];
    ASSERT_EQ(value3, readStream_.readNext<uint8_t>());
}

TEST_F(ReadStreamTest, readNextString)
{
    size_t string1Length = 10;
    std::string string1(reinterpret_cast<const char*>(&samplePayloads_.fullFilledPayload_[6]), string1Length);
    ASSERT_EQ(string1, readStream_.readNextString(string1Length));

    size_t string2Length = 20;
    std::string string2(reinterpret_cast<const char*>(&samplePayloads_.fullFilledPayload_[16]), string2Length);
    ASSERT_EQ(string2, readStream_.readNextString(string2Length));
}

TEST_F(ReadStreamTest, readNextWideString)
{
    size_t string1Length = 10;
    std::wstring string1(reinterpret_cast<const wchar_t*>(&samplePayloads_.fullFilledPayload_[6]), string1Length);
    ASSERT_EQ(string1, readStream_.readNextWideString(string1Length));

    size_t string2Length = 20;
    std::wstring string2(reinterpret_cast<const wchar_t*>(&samplePayloads_.fullFilledPayload_[46]), string2Length);
    ASSERT_EQ(string2, readStream_.readNextWideString(string2Length));
}

TEST_F(ReadStreamTest, getId)
{
    uint16_t &id = reinterpret_cast<uint16_t&>(samplePayloads_.fullFilledPayload_[4]);
    ASSERT_EQ(id, readStream_.getId());
}

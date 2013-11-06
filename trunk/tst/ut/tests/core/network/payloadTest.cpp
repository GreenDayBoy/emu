#include <gtest/gtest.h>

#include <core/network/payload.hpp>

namespace network = eMU::core::network;

class PayloadTest: public ::testing::Test
{
public:
    PayloadTest() {}

    void insertSampleDataToPayload()
    {
        payload_.setValue<uint8_t>(0, 0x01);
        payload_.setValue<uint16_t>(1, 0x0302);
        payload_.setValue<uint32_t>(3, 0x07060504);
    }

protected:
    network::Payload payload_;
};

TEST_F(PayloadTest, checkMaxSize)
{
    ASSERT_EQ(4096, network::Payload::getMaxSize());
}

TEST_F(PayloadTest, setSizeShouldThrowExceptionWhenValueIsOutOfBound)
{
    bool exceptionThrown = false;

    try
    {
        payload_.setSize(network::Payload::getMaxSize() + 1);
    }
    catch(const network::Payload::SizeOutOfBoundException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PayloadTest, construct)
{
    ASSERT_EQ(0, payload_.getSize());
}

TEST_F(PayloadTest, setValue)
{
    network::Payload::Data data(7, 0);
    data[0] = 0x01;
    data[1] = 0x02;
    data[2] = 0x03;
    data[3] = 0x04;
    data[4] = 0x05;
    data[5] = 0x06;
    data[6] = 0x07;

    insertSampleDataToPayload();

    ASSERT_EQ(data.size(), payload_.getSize());
    ASSERT_EQ(memcmp(&payload_[0], &data[0], payload_.getSize()), 0);
}

TEST_F(PayloadTest, throwExceptionWhenOffsetInSetValueIsOutOfBound)
{
    bool exceptionThrown = false;

    try
    {
        payload_.setValue<uint16_t>(network::Payload::getMaxSize() - 1, 0xFFFF);
    }
    catch(const network::Payload::SetOverflowException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PayloadTest, getValue)
{
    insertSampleDataToPayload();

    ASSERT_EQ(0x04030201, payload_.getValue<uint32_t>(0));
    ASSERT_EQ(0x0605, payload_.getValue<uint16_t>(4));
    ASSERT_EQ(0x07, payload_.getValue<uint8_t>(6));
}

TEST_F(PayloadTest, throwExceptionWhenOffsetInGetValueIsOutOfBound)
{
    bool exceptionThrown = false;

    try
    {
        payload_.getValue<uint32_t>(network::Payload::getMaxSize() - 3);
    }
    catch(const network::Payload::GetOverflowException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

#include <gtest/gtest.h>

#include <core/network/payload.hpp>

namespace network = eMU::core::network;

class PayloadTest: public ::testing::Test
{
public:
    PayloadTest() {}

    void insertSampleDataToPayload()
    {
        payload_.insert<uint8_t>(0x01);
        payload_.insert<uint16_t>(0x0302);
        payload_.insert<uint32_t>(0x07060504);

        payload_.setSize(7);
    }

protected:
    network::Payload payload_;
};

TEST_F(PayloadTest, checkMaxSize)
{
    ASSERT_EQ(4096, network::Payload::getMaxSize());
}

TEST_F(PayloadTest, empty)
{
    ASSERT_TRUE(payload_.empty());

    payload_.insert<uint8_t>(0x30);
    ASSERT_FALSE(payload_.empty());
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

TEST_F(PayloadTest, insert)
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

TEST_F(PayloadTest, throwExceptionWhenOffsetInInsertIsOutOfBound)
{
    bool exceptionThrown = false;

    try
    {
        for(size_t i = 0; i < network::Payload::getMaxSize(); ++i)
        {
            payload_.insert<uint8_t>(0xFF);
        }

        payload_.insert<uint8_t>(0xFF);
    }
    catch(const network::Payload::InsertOverflowException&)
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
        payload_.getValue<uint32_t>(0);
    }
    catch(const network::Payload::GetOverflowException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

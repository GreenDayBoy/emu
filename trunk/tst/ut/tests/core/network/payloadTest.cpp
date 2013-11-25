#include <gtest/gtest.h>

#include <core/network/payload.hpp>

using eMU::core::network::Payload;

class PayloadTest: public ::testing::Test
{
public:
    PayloadTest() {}

protected:
    Payload payload_;
};

TEST_F(PayloadTest, checkMaxSize)
{
    ASSERT_EQ(4096, Payload::getMaxSize());
}

TEST_F(PayloadTest, empty)
{
    ASSERT_TRUE(payload_.empty());

    payload_.setSize(1);
    ASSERT_FALSE(payload_.empty());
}

TEST_F(PayloadTest, setSizeShouldThrowExceptionWhenValueIsOutOfBound)
{
    bool exceptionThrown = false;

    try
    {
        payload_.setSize(Payload::getMaxSize() + 1);
    }
    catch(const Payload::SizeOutOfBoundException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(PayloadTest, construct)
{
    ASSERT_EQ(0, payload_.getSize());
}

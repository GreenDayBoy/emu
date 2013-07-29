#include <gtest/gtest.h>

#include <core/protocol/packetsExtractor.hpp>
#include <core/protocol/exceptions.hpp>

namespace network = eMU::core::network;
namespace protocol = eMU::core::protocol;

TEST(PacketsExtractorTest, extract)
{
    network::Payload payload1 = {0xC1, 0x05, 0x01, 0x02, 0x03};
    network::Payload payload2 = {0xC2, 0x08, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08};
    network::Payload payload3 = {0xC3, 0x03, 0x09};
    network::Payload payload4 = {0xC4, 0x0C, 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12};

    network::Payload payload;
    payload.insert(payload.end(), payload1.begin(), payload1.end());
    payload.insert(payload.end(), payload2.begin(), payload2.end());
    payload.insert(payload.end(), payload3.begin(), payload3.end());
    payload.insert(payload.end(), payload4.begin(), payload4.end());

    protocol::PacketsExtractor extractor(payload);
    extractor.extract();

    ASSERT_EQ(4, extractor.payloads().size());

    EXPECT_EQ(payload1, extractor.payloads()[0]);
    EXPECT_EQ(payload2, extractor.payloads()[1]);
    EXPECT_EQ(payload3, extractor.payloads()[2]);
    EXPECT_EQ(payload4, extractor.payloads()[3]);
}

TEST(PacketsExtractorTest, gotEmptyPayloadShouldThrowException)
{
    bool exceptionThrown = false;

    try
    {
        network::Payload payload;
        protocol::PacketsExtractor exctractor(payload);
        exctractor.extract();

    }
    catch(protocol::exceptions::EmptyPayloadException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST(PacketsExtractorTest, ToLargePacketSizeShouldThrowException)
{
    network::Payload payload1 = {0xC1, 0x05, 0x01, 0x02, 0x03};
    network::Payload payload2 = {0xC2, 0x09, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08};

    network::Payload payload;
    payload.insert(payload.end(), payload1.begin(), payload1.end());
    payload.insert(payload.end(), payload2.begin(), payload2.end());

    bool exceptionThrown = false;

    try
    {
        protocol::PacketsExtractor exctractor(payload);
        exctractor.extract();

    }
    catch(protocol::exceptions::InvalidPacketSizeException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST(PacketsExtractorTest, ToSmallPacketSizeShouldThrowException)
{
    network::Payload payload1 = {0xC1, 0x04, 0x01, 0x02, 0x03};
    network::Payload payload2 = {0xC2, 0x09, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08};

    network::Payload payload;
    payload.insert(payload.end(), payload1.begin(), payload1.end());
    payload.insert(payload.end(), payload2.begin(), payload2.end());

    bool exceptionThrown = false;

    try
    {
        protocol::PacketsExtractor exctractor(payload);
        exctractor.extract();

    }
    catch(protocol::exceptions::InvalidPacketHeaderException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST(PacketsExtractorTest, InvalidPacketHeaderShouldThrowException)
{
    network::Payload payload1 = {0xC1, 0x05, 0x01, 0x02, 0x03};
    network::Payload payload2 = {0xC5};

    network::Payload payload;
    payload.insert(payload.end(), payload1.begin(), payload1.end());
    payload.insert(payload.end(), payload2.begin(), payload2.end());

    bool exceptionThrown = false;

    try
    {
        protocol::PacketsExtractor exctractor(payload);
        exctractor.extract();

    }
    catch(protocol::exceptions::InvalidPacketHeaderException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

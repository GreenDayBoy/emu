//#include <gtest/gtest.h>

//#include <core/network/buffer.hpp>
//#include <core/protocol/helpers.hpp>
//#include <interface/header.hpp>

//namespace network = eMU::core::network;
//namespace protocol = eMU::core::protocol;
//namespace interface = eMU::interface;

//TEST(HelpersTest, byteSwap)
//{
//    EXPECT_EQ(0xFFFF, protocol::byteSwap(0xFFFF));
//    EXPECT_EQ(0x30FF, protocol::byteSwap(0xFF30));
//    EXPECT_EQ(0x0050, protocol::byteSwap(0x5000));
//    EXPECT_EQ(0x4600, protocol::byteSwap(0x0046));
//    EXPECT_EQ(0x0000, protocol::byteSwap(0x0000));
//}

//TEST(HelpersTest, getersForSmallMessage)
//{
//    network::Payload payload = {0xC1, 0x30, 0xF4};
//    EXPECT_EQ(payload[2], protocol::getProtocolId(payload));
//    EXPECT_EQ(payload[1], protocol::getMessageSize(payload));

//    payload[0] = 0xC3;
//    EXPECT_EQ(payload[2], protocol::getProtocolId(payload));
//    EXPECT_EQ(payload[1], protocol::getMessageSize(payload));
//}

//TEST(HelpersTest, getersForLargeMessage)
//{
//    network::Payload payload = {0xC2, 0x30, 0xFF, 0xF4};
//    EXPECT_EQ(payload[3], protocol::getProtocolId(payload));
//    EXPECT_EQ(0xFF30, protocol::getMessageSize(payload));

//    payload[0] = 0xC4;
//    EXPECT_EQ(payload[3], protocol::getProtocolId(payload));
//    EXPECT_EQ(0xFF30, protocol::getMessageSize(payload));
//}

//TEST(HelpersTest, getersForInvalidMessage)
//{
//    network::Payload payload = {0xFF, 0x30, 0xFF, 0xF4};
//    EXPECT_EQ(0, protocol::getProtocolId(payload));
//    EXPECT_EQ(0, protocol::getMessageSize(payload));
//}

//TEST(HelpersTest, isCrypted)
//{
//    network::Payload payload = {0xC3, 0x04, 0x00, 0x00};
//    EXPECT_TRUE(protocol::isCrypted(payload));

//    payload[0] = 0xC4;
//    EXPECT_TRUE(protocol::isCrypted(payload));

//    payload[0] = 0xC1;
//    EXPECT_FALSE(protocol::isCrypted(payload));

//    payload[0] = 0xC2;
//    EXPECT_FALSE(protocol::isCrypted(payload));
//}

//TEST(HelpersTest, isValid)
//{
//    network::Payload payload = {0xC1, 0x04, 0x00, 0x00};
//    EXPECT_TRUE(protocol::hasValidHeader(payload));

//    payload[0] = 0xC2;
//    EXPECT_TRUE(protocol::hasValidHeader(payload));

//    payload[0] = 0xC3;
//    EXPECT_TRUE(protocol::hasValidHeader(payload));

//    payload[0] = 0xC4;
//    EXPECT_TRUE(protocol::hasValidHeader(payload));

//    payload[0] = 0xC5;
//    EXPECT_FALSE(protocol::hasValidHeader(payload));
//}

//TEST(HelpersTest, getHeaderSize)
//{
//    network::Payload payloadSmall = {0xC1, 0x04, 0x00, 0x00};
//    EXPECT_EQ(sizeof(interface::SmallMessageHeader), protocol::getHeaderSize(payloadSmall));

//    payloadSmall[0] = 0xC3;
//    EXPECT_EQ(sizeof(interface::SmallMessageHeader), protocol::getHeaderSize(payloadSmall));

//    network::Payload payloadLarge = {0xC2, 0x00, 0x05, 0x00, 0x00};
//    EXPECT_EQ(sizeof(interface::LargeMessageHeader), protocol::getHeaderSize(payloadLarge));

//    payloadLarge[0] = 0xC4;
//    EXPECT_EQ(sizeof(interface::LargeMessageHeader), protocol::getHeaderSize(payloadLarge));

//    network::Payload invalidPayload = {0xC5, 0x04, 0x00, 0x00};
//    EXPECT_EQ(0, protocol::getHeaderSize(invalidPayload));
//}

//TEST(HelpersTest, getMessageId)
//{
//    network::Payload payload1 = {0xC1, 0x04, 0x00, 0x01};
//    EXPECT_EQ(payload1[3], protocol::getMessageId(payload1));

//    network::Payload payload2 = {0xC3, 0x06, 0x00, 0xFF, 0x03, 0x40};
//    EXPECT_EQ(payload2[3], protocol::getMessageId(payload2));

//    network::Payload payload3 = {0xC2, 0x00, 0x06, 0xDD, 0x01, 0x22};
//    EXPECT_EQ(payload3[4], protocol::getMessageId(payload3));

//    network::Payload payload4 = {0xC4, 0x00, 0x05, 0xBC, 0x01};
//    EXPECT_EQ(payload4[4], protocol::getMessageId(payload4));
//}

//TEST(HelpersTest, getMessageIdShouldReturn0WhenMessageIsToShortOrHeaderIsInvalid)
//{
//    network::Payload payload1 = {0xC1, 0x02};
//    EXPECT_EQ(0, protocol::getMessageId(payload1));

//    network::Payload payload2 = {0xC3, 0x03, 0xEE};
//    EXPECT_EQ(0, protocol::getMessageId(payload2));

//    network::Payload payload3 = {0xC2, 0x00, 0x03};
//    EXPECT_EQ(0, protocol::getMessageId(payload3));

//    network::Payload payload4 = {0xC4, 0x00, 0x04, 0xEF};
//    EXPECT_EQ(0, protocol::getMessageId(payload4));

//    network::Payload payload5 = {0xC5, 0x04, 0x04, 0xEF};
//    EXPECT_EQ(0, protocol::getMessageId(payload5));
//}

#include <protocol/dataserver/encoders/faultIndication.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/readStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace protocol = eMU::protocol;

TEST(FaultIndicationTest, encode)
{   
    size_t clientHash = 0x4567;
    std::string message = "Test fault message";
    protocol::dataserver::encoders::FaultIndication indication(clientHash, message);

    // -----------------------------------------------------------------------------

    eMU::protocol::ReadStream readStream(indication.getWriteStream().getPayload());

    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kFaultIndication, readStream.getId());
    ASSERT_EQ(clientHash, readStream.readNext<size_t>());

    size_t messageLength = readStream.readNext<uint32_t>();
    ASSERT_EQ(message.length(), messageLength);
    ASSERT_EQ(message, readStream.readNextString(messageLength));
}

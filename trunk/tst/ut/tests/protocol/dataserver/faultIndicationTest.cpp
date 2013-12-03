#include <protocol/dataserver/faultIndication.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>
#include <protocol/readStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::ReadStream;
using eMU::protocol::WriteStream;
using eMU::protocol::dataserver::FaultIndication;

TEST(FaultIndicationTest, decode)
{   
    WriteStream writeStream(MessageIds::kFaultIndication);

    size_t clientHash = 0x123456;
    writeStream.writeNext<size_t>(clientHash);

    std::string message = "test message";
    writeStream.writeNext<uint32_t>(message.length());
    writeStream.writeNext(message);

    // -----------------------------------------------------------------------------

    FaultIndication indication(ReadStream(writeStream.getPayload()));

    ASSERT_EQ(clientHash, indication.getClientHash());
    ASSERT_EQ(message, indication.getMessage());
}

TEST(FaultIndicationTest, encode)
{
    size_t clientHash = 0x4567;
    std::string message = "Test fault message";
    FaultIndication indication(clientHash, message);

    // -----------------------------------------------------------------------------

    eMU::protocol::ReadStream readStream(indication.getWriteStream().getPayload());

    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kFaultIndication, readStream.getId());
    ASSERT_EQ(clientHash, readStream.readNext<size_t>());

    size_t messageLength = readStream.readNext<uint32_t>();
    ASSERT_EQ(message.length(), messageLength);
    ASSERT_EQ(message, readStream.readNextString(messageLength));
}

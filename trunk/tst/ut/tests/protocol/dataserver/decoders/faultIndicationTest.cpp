#include <protocol/dataserver/decoders/faultIndication.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::ReadStream;
using eMU::protocol::WriteStream;
using eMU::protocol::dataserver::decoders::FaultIndication;

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

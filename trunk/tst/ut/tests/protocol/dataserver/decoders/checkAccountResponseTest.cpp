#include <protocol/dataserver/decoders/checkAccountResponse.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::WriteStream;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::decoders::CheckAccountResponse;
using eMU::protocol::dataserver::CheckAccountResult;

TEST(CheckAccountResponseTest, decode)
{
    WriteStream writeStream(MessageIds::kCheckAccountResponse);

    size_t clientHash = 0x123456;
    writeStream.writeNext<size_t>(clientHash);

    CheckAccountResult result = CheckAccountResult::Failed;
    writeStream.writeNext<CheckAccountResult>(result);

    ReadStream readStream(writeStream.getPayload());
    CheckAccountResponse response(readStream);

    ASSERT_EQ(clientHash, response.getClientHash());
    ASSERT_EQ(result, response.getResult());
}

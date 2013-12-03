#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::WriteStream;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResponse;
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

TEST(CheckAccountResponseTest, encode)
{
    size_t clientHash = 0x4567;
    CheckAccountResult result = CheckAccountResult::Succeed;
    CheckAccountResponse response(clientHash, result);

    // -----------------------------------------------------------------------------

    ReadStream readStream(response.getWriteStream().getPayload());

    ASSERT_EQ(MessageIds::kCheckAccountResponse, readStream.getId());
    ASSERT_EQ(clientHash, readStream.readNext<size_t>());
    ASSERT_EQ(static_cast<uint8_t>(result), readStream.readNext<uint8_t>());
}

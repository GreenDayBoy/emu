#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/readStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::encoders::CheckAccountResponse;

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

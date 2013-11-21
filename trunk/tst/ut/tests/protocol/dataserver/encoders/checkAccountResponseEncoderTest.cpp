#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/dataserver/CheckAccountResult.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/readStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace protocol = eMU::protocol;

TEST(CheckAccountResponseEncoderTest, encode)
{   
    size_t clientHash = 0x4567;
    protocol::dataserver::CheckAccountResult result = protocol::dataserver::CheckAccountResult::Succeed;
    protocol::dataserver::encoders::CheckAccountResponse response(clientHash, result);

    eMU::protocol::ReadStream readStream(response.getWriteStream().getPayload());

    ASSERT_EQ(eMU::protocol::dataserver::MessageIds::kCheckAccountResponse, readStream.getId());
    ASSERT_EQ(clientHash, readStream.readNext<size_t>());
    ASSERT_EQ(static_cast<uint8_t>(result), readStream.readNext<uint8_t>());
}

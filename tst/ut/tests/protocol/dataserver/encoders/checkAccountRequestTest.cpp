#include <protocol/dataserver/encoders/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/readStream.hpp>

#include <gtest/gtest.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::encoders::CheckAccountRequest;

TEST(CheckAccountRequestTest, encode)
{   
    size_t clientHash = 0x4567;
    std::string accountId = "account";
    std::string password = "password";
    CheckAccountRequest request(clientHash, accountId, password);

    // -----------------------------------------------------------------------------

    ReadStream readStream(request.getWriteStream().getPayload());

    ASSERT_EQ(MessageIds::kCheckAccountRequest, readStream.getId());
    ASSERT_EQ(clientHash, readStream.readNext<size_t>());

    ASSERT_EQ(accountId.length(), readStream.readNext<uint32_t>());
    ASSERT_EQ(accountId, readStream.readNextString(accountId.length()));

    ASSERT_EQ(password.length(), readStream.readNext<uint32_t>());
    ASSERT_EQ(password, readStream.readNextString(password.length()));
}

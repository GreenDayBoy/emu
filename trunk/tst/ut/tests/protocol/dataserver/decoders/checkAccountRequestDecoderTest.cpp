#include <protocol/dataserver/decoders/checkAccountRequest.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace protocol = eMU::protocol;

TEST(CheckAccountRequestDecoderTest, decode)
{   
    protocol::WriteStream writeStream(protocol::dataserver::MessageIds::kCheckAccountRequest);

    size_t clientHash = 0x123456;
    writeStream.writeNext<size_t>(clientHash);

    std::string accountId = "testAccount";
    writeStream.writeNext<uint32_t>(accountId.length());
    writeStream.writeNext(accountId);

    std::string password = "testAccount";
    writeStream.writeNext<uint32_t>(password.length());
    writeStream.writeNext(password);

    protocol::ReadStream readStream(writeStream.getPayload());
    protocol::dataserver::decoders::CheckAccountRequest request(readStream);

    ASSERT_EQ(clientHash, request.getClientHash());
    ASSERT_EQ(accountId, request.getAccountId());
    ASSERT_EQ(password, request.getPassword());
}

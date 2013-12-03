#include <protocol/dataserver/checkAccountRequest.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::dataserver::MessageIds;
using eMU::protocol::WriteStream;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::CheckAccountResult;
using eMU::protocol::dataserver::CheckAccountRequest;

TEST(CheckAccountRequestTest, decode)
{   
    WriteStream writeStream(MessageIds::kCheckAccountRequest);

    size_t clientHash = 0x123456;
    writeStream.writeNext<size_t>(clientHash);

    std::string accountId = "testAccount";
    writeStream.writeNext<uint32_t>(accountId.length());
    writeStream.writeNext(accountId);

    std::string password = "testAccount";
    writeStream.writeNext<uint32_t>(password.length());
    writeStream.writeNext(password);

    // -----------------------------------------------------------------------------

    CheckAccountRequest request(ReadStream(writeStream.getPayload()));

    ASSERT_EQ(clientHash, request.getClientHash());
    ASSERT_EQ(accountId, request.getAccountId());
    ASSERT_EQ(password, request.getPassword());
}

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

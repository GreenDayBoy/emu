#include <protocol/loginserver/decoders/loginRequest.hpp>
#include <protocol/loginserver/messageIds.hpp>
#include <protocol/writeStream.hpp>

#include <gtest/gtest.h>
#include <stdint.h>

namespace MessageIds = eMU::protocol::loginserver::MessageIds;
using eMU::protocol::WriteStream;
using eMU::protocol::ReadStream;
using eMU::protocol::loginserver::decoders::LoginRequest;

TEST(LoginRequestTest, decode)
{   
    WriteStream writeStream(MessageIds::kLoginRequest);

    writeStream.writeNext<uint32_t>(0); // dummy1;
    writeStream.writeNext<uint32_t>(0); // dummy1;

    std::wstring accountId = L"mu2emulator";
    writeStream.writeNext<uint32_t>(accountId.length());
    writeStream.writeNext(accountId);

    std::wstring password = L"asdasdasd";
    writeStream.writeNext<uint32_t>(password.length());
    writeStream.writeNext(password);

    // -----------------------------------------------------------------------------

    LoginRequest request(ReadStream(writeStream.getPayload()));

    ASSERT_EQ("mu2emulator", request.getAccountId());
    ASSERT_EQ("asdasdasd", request.getPassword());
}

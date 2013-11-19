//#include <gtest/gtest.h>
//#include <interface/dataserver/decoders/checkAccountRequest.hpp>

//TEST(CheckAccountRequestTest, decode)
//{
//    eMU::core::network::Payload packet;

//    size_t clientHash = 123456;
//    packet.insert<size_t>(clientHash);

//    std::string accountId = "testAccount";
//    packet.insert<uint32_t>(accountId.length());
//    packet.insert(accountId);

//    std::string password = "testPassword";
//    packet.insert<uint32_t>(password.length());
//    packet.insert(password);

//    eMU::interface::dataserver::decoders::CheckAccountRequest request(packet);

//    ASSERT_EQ(clientHash, request.getClientHash());
//    ASSERT_EQ(accountId, request.getAccountId());
//    ASSERT_EQ(password, request.getPassword());
//}

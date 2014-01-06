#include <gameserver/transactions/worldLoginRequest.hpp>
#include <gameserver/user.hpp>

#include <streaming/readStream.hpp>
#include <streaming/gameserver/streamIds.hpp>
#include <streaming/gameserver/worldLoginRequest.hpp>
#include <streaming/gameserver/worldLoginResponse.hpp>

#include <ut/env/core/network/tcp/connectionMock.hpp>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;

using eMU::ut::env::core::network::tcp::ConnectionMock;
using eMU::core::network::Payload;
using eMU::streaming::ReadStream;
using eMU::gameserver::User;
using eMU::streaming::gameserver::WorldLoginRequest;
using eMU::streaming::gameserver::WorldLoginResponse;
namespace streamIds = eMU::streaming::gameserver::streamIds;

class WorldLoginRequestTransactionTest: public ::testing::Test
{
protected:
    WorldLoginRequestTransactionTest():
        request_(ReadStream(eMU::streaming::gameserver::WorldLoginRequest().getWriteStream().getPayload())),
        connection_(new ConnectionMock()),
        user_(connection_) {}

    eMU::streaming::gameserver::WorldLoginRequest request_;
    ConnectionMock::Pointer connection_;
    User user_;
};

TEST_F(WorldLoginRequestTransactionTest, handle)
{
    Payload payload;
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload));

    eMU::gameserver::transactions::WorldLoginRequest(user_, request_).handle();

    ReadStream readStream(payload);
    ASSERT_EQ(streamIds::kWorldLoginResponse, readStream.getId());

    WorldLoginResponse response(readStream);
    ASSERT_EQ(0, response.getResult());
}

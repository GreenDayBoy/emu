#include <gameserver/protocol.hpp>
#include <gameserver/context.hpp>

#include <ut/asioStub/ioService.hpp>

#include <gtest/gtest.h>

using eMU::core::network::tcp::Connection;
using eMU::core::network::tcp::NetworkUser;
using eMU::gameserver::Protocol;
using eMU::gameserver::Context;
using eMU::ut::env::asioStub::io_service;

class GameserverProtocolTest: public ::testing::Test
{
protected:
    GameserverProtocolTest():
        context_(5, 12),
        protocol_(context_),
        connection_(new Connection(ioService_, protocol_)) {}

    Context context_;
    Protocol protocol_;
    io_service ioService_;
    Connection::Pointer connection_;
};

TEST_F(GameserverProtocolTest, WhenRegistrationInfoIsEmptyThenAttachShouldFailed)
{
    ASSERT_FALSE(protocol_.attach(connection_));
    ASSERT_TRUE(context_.getUsersFactory().getObjects().empty());
}

TEST_F(GameserverProtocolTest, WhenRegistrationInfoIsPresentThenAttachShouldSucceed)
{
    std::string accountId = "sampleLogin";
    context_.getUserRegistrationInfos().push_back({NetworkUser::Hash(0x621), accountId});

    ASSERT_TRUE(protocol_.attach(connection_));

    ASSERT_EQ(1, context_.getUsersFactory().getObjects().size());
    ASSERT_EQ(accountId, context_.getUsersFactory().getObjects().back()->getAccountId());
    ASSERT_TRUE(context_.getUserRegistrationInfos().empty());
}

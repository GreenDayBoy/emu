#include <gameserver/transactions/faultIndication.hpp>
#include <gameserver/user.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <ut/core/network/tcp/connectionMock.hpp>

using eMU::gameserver::User;
using eMU::core::common::Factory;
using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::FaultIndication;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class GameserverFaultIndicationTransactionTest: public ::testing::Test
{
protected:
    GameserverFaultIndicationTransactionTest():
        userHashExists_(false) {}

    void scenario()
    {
        User::Hash hash(0x12345);
        ConnectionMock::Pointer connection(new ConnectionMock());

        if(userHashExists_)
        {
            User &user = usersFactory_.create(connection);
            hash = user.getHash();

            EXPECT_CALL(*connection, disconnect());
        }

        FaultIndication indication(hash, "testMessage");
        eMU::gameserver::transactions::FaultIndication(usersFactory_, ReadStream(indication.getWriteStream().getPayload())).handle();
    }

    Factory<User> usersFactory_;
    bool userHashExists_;
};

TEST_F(GameserverFaultIndicationTransactionTest, WhenUserHashIsValidThenConnectionShouldBeDisconnect)
{
    userHashExists_ = true;
    scenario();
}

TEST_F(GameserverFaultIndicationTransactionTest, WhenUserHashIsInvalidThenNothingHappens)
{
    scenario();
}

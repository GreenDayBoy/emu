#include <loginserver/transactions/faultIndication.hpp>
#include <loginserver/user.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

using eMU::loginserver::User;
using eMU::core::common::Factory;
using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::FaultIndication;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class FaultIndicationTransactionTest: public ::testing::Test
{
protected:
    FaultIndicationTransactionTest():
        clientHashExists_(false) {}

    void scenario()
    {
        User::Hash hash(0x12345);
        ConnectionMock::Pointer connection(new ConnectionMock());

        if(clientHashExists_)
        {
            User &user = usersFactory_.create(connection);
            hash = user.getHash();

            EXPECT_CALL(*connection, disconnect());
        }

        FaultIndication indication(hash, "testMessage");
        eMU::loginserver::transactions::FaultIndication(usersFactory_, ReadStream(indication.getWriteStream().getPayload())).handle();
    }

    Factory<User> usersFactory_;
    bool clientHashExists_;
};

TEST_F(FaultIndicationTransactionTest, WhenClientHashIsValidThenConnectionShouldBeDisconnect)
{
    clientHashExists_ = true;
    scenario();
}

TEST_F(FaultIndicationTransactionTest, WhenClientHashIsInvalidThenNothingHappens)
{
    scenario();
}

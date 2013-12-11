#include <loginserver/transactions/faultIndicationTransaction.hpp>
#include <loginserver/user.hpp>
#include <protocol/dataserver/faultIndication.hpp>
#include <ut/env/core/network/tcp/connectionMock.hpp>

using eMU::loginserver::User;
using eMU::loginserver::transactions::FaultIndicationTransaction;
using eMU::core::common::Factory;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::FaultIndication;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class FaultIndicationTransactionTest: public ::testing::Test
{
protected:
    FaultIndicationTransactionTest():
        clientHashExists_(false) {}

    void scenario()
    {
        User::Hash hash(0x12345);
        ConnectionMock connection;

        if(clientHashExists_)
        {
            User &user = usersFactory_.create(connection);
            hash = user.getHash();

            EXPECT_CALL(connection, disconnect());
        }

        FaultIndication indication(hash, "testMessage");
        FaultIndicationTransaction(usersFactory_, ReadStream(indication.getWriteStream().getPayload())).handle();
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

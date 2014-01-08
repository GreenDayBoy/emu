#include <loginserver/transactions/faultIndication.hpp>
#include <loginserver/user.hpp>
#include <streaming/dataserver/faultIndication.hpp>
#include <ut/core/network/tcp/connectionMock.hpp>

using eMU::loginserver::User;
using eMU::core::common::Factory;
using eMU::streaming::ReadStream;
using eMU::streaming::dataserver::FaultIndication;
using eMU::ut::env::core::network::tcp::ConnectionMock;

class LoginserverFaultIndicationTransactionTest: public ::testing::Test
{
protected:
    LoginserverFaultIndicationTransactionTest():
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
        eMU::loginserver::transactions::FaultIndication(usersFactory_, ReadStream(indication.getWriteStream().getPayload())).handle();
    }

    Factory<User> usersFactory_;
    bool userHashExists_;
};

TEST_F(LoginserverFaultIndicationTransactionTest, WhenUserHashIsValidThenConnectionShouldBeDisconnect)
{
    userHashExists_ = true;
    scenario();
}

TEST_F(LoginserverFaultIndicationTransactionTest, WhenUserHashIsInvalidThenNothingHappens)
{
    scenario();
}

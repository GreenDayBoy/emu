#include <loginserver/transactions/faultIndicationTransaction.hpp>
#include <loginserver/user.hpp>
#include <core/common/usersFactory.hpp>
#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <protocol/dataserver/faultIndication.hpp>

using eMU::loginserver::User;
using eMU::loginserver::transactions::FaultIndicationTransaction;
using eMU::core::common::UsersFactory;
using eMU::ut::env::core::network::tcp::ConnectionsManagerMock;
using eMU::protocol::ReadStream;
using eMU::protocol::dataserver::FaultIndication;

class FaultIndicationTransactionTest: public ::testing::Test
{
protected:
    FaultIndicationTransactionTest():
        usersFactory_(1),
        clientHashExists_(false) {}

    void scenario()
    {
        size_t hash = 0x12345;

        if(clientHashExists_)
        {
            hash = usersFactory_.create();
            EXPECT_CALL(connectionsManager_, disconnect(hash));
        }

        FaultIndication indication(hash, "testMessage");
        FaultIndicationTransaction(connectionsManager_, usersFactory_, ReadStream(indication.getWriteStream().getPayload())).handle();
    }

    UsersFactory<User> usersFactory_;
    ConnectionsManagerMock connectionsManager_;
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

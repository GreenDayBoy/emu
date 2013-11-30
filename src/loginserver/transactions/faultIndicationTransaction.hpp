#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <core/common/usersFactory.hpp>

#include <loginserver/user.hpp>

#include <protocol/dataserver/decoders/faultIndication.hpp>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

class FaultIndicationTransaction: public core::transactions::Transaction
{
public:
    FaultIndicationTransaction(core::network::tcp::ConnectionsManager &connectionsManager,
                               core::common::UsersFactory<User> &usersFactory,
                               const protocol::dataserver::decoders::FaultIndication &indication);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    core::network::tcp::ConnectionsManager &connectionsManager_;
    core::common::UsersFactory<User> &usersFactory_;
    protocol::dataserver::decoders::FaultIndication indication_;
};

}
}
}

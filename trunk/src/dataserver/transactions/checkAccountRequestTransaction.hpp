#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>

#include <dataserver/database/sqlInterface.hpp>

#include <protocol/dataserver/checkAccountRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequestTransaction: public core::transactions::Transaction
{
public:
    CheckAccountRequestTransaction(size_t hash,
                                   database::SqlInterface &sqlInterface,
                                   core::network::tcp::ConnectionsManager &connectionsManager,
                                   const protocol::dataserver::CheckAccountRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    void sendFaultIndication(const std::string &message);

    size_t hash_;
    database::SqlInterface &sqlInterface_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    protocol::dataserver::CheckAccountRequest request_;
};

}
}
}

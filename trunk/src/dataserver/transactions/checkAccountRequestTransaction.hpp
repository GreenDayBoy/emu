#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/tcp/connectionsManager.hpp>

#include <dataserver/database/sqlInterface.hpp>

#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequestTransaction: public eMU::core::transactions::Transaction
{
public:
    CheckAccountRequestTransaction(size_t hash,
                                   database::SqlInterface &sqlInterface,
                                   core::network::tcp::ConnectionsManager &connectionsManager,
                                   const protocol::dataserver::decoders::CheckAccountRequest &request);

    bool isValid() const;
    void handle();

private:
    void decode();

    size_t hash_;
    database::SqlInterface &sqlInterface_;
    core::network::tcp::ConnectionsManager &connectionsManager_;
    protocol::dataserver::decoders::CheckAccountRequest request_;
};

}
}
}

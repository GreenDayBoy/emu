#pragma once

#include <core/transactions/transaction.hpp>
#include <core/network/payload.hpp>
#include <dataserver/database/sqlInterface.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequestTransaction: public eMU::core::transactions::Transaction
{
public:
    CheckAccountRequestTransaction(const core::network::Payload &packet, database::SqlInterface &sqlInterface);

    bool isValid() const;
    void handle();

private:
    const core::network::Payload &packet_;
    database::SqlInterface &sqlInterface_;
};

}
}
}

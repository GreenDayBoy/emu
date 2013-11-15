#pragma once

#include <core/transactions/transaction.hpp>
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
    CheckAccountRequestTransaction(database::SqlInterface &sqlInterface);

    bool isValid() const;
    void handle();

private:
    database::SqlInterface &sqlInterface_;
};

}
}
}

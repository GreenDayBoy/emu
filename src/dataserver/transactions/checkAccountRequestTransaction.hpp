#pragma once

#include <core/transactions/transaction.hpp>
#include <dataserver/database/database.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequestTransaction: public eMU::core::transactions::Transaction
{
public:
    CheckAccountRequestTransaction(database::Database &database);

    bool isValid() const;
    void handle();

private:
    database::Database &database_;
};

}
}
}

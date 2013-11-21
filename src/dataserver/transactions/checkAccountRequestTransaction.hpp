#pragma once

#include <core/transactions/transaction.hpp>
#include <protocol/dataserver/decoders/checkAccountRequest.hpp>
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
    CheckAccountRequestTransaction(size_t hash, database::SqlInterface &sqlInterface,
                                   const protocol::dataserver::decoders::CheckAccountRequest &request);

    bool isValid() const;
    void handle();

private:
    void decode();

    size_t hash_;
    database::SqlInterface &sqlInterface_;
    const protocol::dataserver::decoders::CheckAccountRequest &request_;
};

}
}
}

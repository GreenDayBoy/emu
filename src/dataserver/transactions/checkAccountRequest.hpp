#pragma once

#include <dataserver/transactions/databaseTransaction.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequest: public DatabaseTransaction
{
public:
    CheckAccountRequest(User &user,
                        database::SqlInterface &sqlInterface,
                        const streaming::dataserver::CheckAccountRequest &request);

private:
    void handleValid();

    streaming::dataserver::CheckAccountRequest request_;
};

}
}
}

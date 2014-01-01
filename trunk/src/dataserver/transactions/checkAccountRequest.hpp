#pragma once

#include <dataserver/user.hpp>
#include <core/transactions/transaction.hpp>
#include <dataserver/database/sqlInterface.hpp>
#include <streaming/dataserver/checkAccountRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CheckAccountRequest: public core::transactions::Transaction
{
public:
    CheckAccountRequest(User &user,
                        database::SqlInterface &sqlInterface,
                        const streaming::dataserver::CheckAccountRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    void sendFaultIndication(const std::string &message);

    User &user_;
    database::SqlInterface &sqlInterface_;
    streaming::dataserver::CheckAccountRequest request_;
};

}
}
}

#pragma once

#include <dataserver/user.hpp>
#include <core/common/transaction.hpp>
#include <dataserver/database/sqlInterface.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class DatabaseTransaction: public core::common::Transaction
{
public:
    DatabaseTransaction(User &user,
                        database::SqlInterface &sqlInterface,
                        core::network::tcp::NetworkUser::Hash userHash);

protected:
    bool isValid() const;
    void handleInvalid();
    void handleValid();
    void sendFaultIndication(const std::string &message);

    User &user_;
    database::SqlInterface &sqlInterface_;
    core::network::tcp::NetworkUser::Hash userHash_;
};

}
}
}

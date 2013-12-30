#pragma once

#include <core/common/factory.hpp>
#include <core/transactions/manager.hpp>
#include <dataserver/user.hpp>
#include <dataserver/database/sqlInterface.hpp>

namespace eMU
{
namespace dataserver
{

class Context: boost::noncopyable
{
public:
    Context(database::SqlInterface &sqlInterface, size_t maxNumberOfUsers);

    core::common::Factory<User>& getUsersFactory();
    core::transactions::Manager& getTransactionsManager();
    database::SqlInterface& getSqlInterface();
    size_t getMaxNumberOfUsers();

private:
    Context();

    core::common::Factory<User> usersFactory_;
    core::transactions::Manager transactionsManager_;
    database::SqlInterface &sqlInterface_;
    size_t maxNumberOfUsers_;
};

}
}

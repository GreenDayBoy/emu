#include <dataserver/context.hpp>

namespace eMU
{
namespace dataserver
{

Context::Context(database::SqlInterface &sqlInterface, size_t maxNumberOfUsers):
    sqlInterface_(sqlInterface),
    maxNumberOfUsers_(maxNumberOfUsers) {}

core::common::Factory<User>& Context::getUsersFactory()
{
    return usersFactory_;
}

core::transactions::Manager& Context::getTransactionsManager()
{
    return transactionsManager_;
}

database::SqlInterface& Context::getSqlInterface()
{
    return sqlInterface_;
}

size_t Context::getMaxNumberOfUsers()
{
    return maxNumberOfUsers_;
}

}
}

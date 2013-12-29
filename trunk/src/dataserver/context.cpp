#include <dataserver/context.hpp>

namespace eMU
{
namespace dataserver
{

Context::Context(size_t maxNumberOfUsers):
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
    return mySqlInterface_;
}

size_t Context::getMaxNumberOfUsers()
{
    return maxNumberOfUsers_;
}

}
}

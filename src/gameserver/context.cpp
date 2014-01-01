#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

Context::Context(size_t maxNumberOfUsers):
    maxNumberOfUsers_(maxNumberOfUsers),
    dataserverConnection_(nullptr) {}

core::common::Factory<User>& Context::getUsersFactory()
{
    return usersFactory_;
}

core::transactions::Manager& Context::getTransactionsManager()
{
    return transactionsManager_;
}

size_t Context::getMaxNumberOfUsers()
{
    return maxNumberOfUsers_;
}

void Context::setDataserverConnection(core::network::tcp::Connection::Pointer connection)
{
    dataserverConnection_ = connection;
}

core::network::tcp::Connection::Pointer Context::getDataserverConnection()
{
    return dataserverConnection_;
}

}
}

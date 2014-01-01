#pragma once

#include <core/common/factory.hpp>
#include <core/transactions/manager.hpp>
#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{

class Context: boost::noncopyable
{
public:
    Context(size_t maxNumberOfUsers);

    core::common::Factory<User>& getUsersFactory();
    core::transactions::Manager& getTransactionsManager();
    size_t getMaxNumberOfUsers();
    void setDataserverConnection(core::network::tcp::Connection::Pointer connection);
    core::network::tcp::Connection::Pointer getDataserverConnection();

private:
    Context();

    core::common::Factory<User> usersFactory_;
    core::transactions::Manager transactionsManager_;
    size_t maxNumberOfUsers_;
    core::network::tcp::Connection::Pointer dataserverConnection_;
};

}
}

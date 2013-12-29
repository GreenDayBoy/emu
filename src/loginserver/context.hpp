#pragma once

#include <core/common/factory.hpp>
#include <core/transactions/manager.hpp>
#include <loginserver/user.hpp>
#include <loginserver/gameserversList.hpp>

namespace eMU
{
namespace loginserver
{

class Context: boost::noncopyable
{
public:
    Context(size_t maxNumberOfUsers);

    core::common::Factory<User>& getUsersFactory();
    core::transactions::Manager& getTransactionsManager();
    GameserversList& getGameserversList();
    size_t getMaxNumberOfUsers();
    void setDataserverConnection(core::network::tcp::Connection::Pointer connection);
    core::network::tcp::Connection::Pointer getDataserverConnection();

private:
    Context();

    core::common::Factory<User> usersFactory_;
    core::transactions::Manager transactionsManager_;
    GameserversList gameserversList_;
    size_t maxNumberOfUsers_;
    core::network::tcp::Connection::Pointer dataserverConnection_;
};

}
}

#pragma once

#include <core/transactions/manager.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>

#include <boost/noncopyable.hpp>
#include <glog/logging.h>

namespace eMU
{
namespace core
{
namespace network
{

template<typename User>
class Server: boost::noncopyable
{
public:
    Server(asio::io_service& ioService, uint16_t port, size_t maxNumberOfUsers):
        connectionsAcceptor_(connectionsFactory_,
                             tcp::ConnectionsAcceptor::AcceptorPointer(new asio::ip::tcp::acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)))),
        maxNumberOfUsers_(maxNumberOfUsers)
    {
           connectionsAcceptor_.setAcceptEventCallback(std::bind(&Server::acceptEvent, this, std::placeholders::_1));
    }

    virtual ~Server() {}

    bool startup()
    {
        if(this->onStartup())
        {
            connectionsAcceptor_.queueAccept();
            return true;
        }

        return false;
    }

    void cleanup()
    {
        this->onCleanup();
    }

    virtual bool onStartup() = 0;
    virtual void onCleanup() = 0;
    virtual void onAccept(User &user) = 0;
    virtual void onReceive(User &user) = 0;
    virtual void onClose(User &user) = 0;

protected:
    void acceptEvent(tcp::Connection &connection)
    {
        if(usersFactory_.getObjects().size() > maxNumberOfUsers_)
        {
            LOG(WARNING) << "Max number of users reached!";

            connection.close();
            connectionsFactory_.destroy(connection);
        }
        else
        {
            User &user = usersFactory_.create(connection);

            connection.setReceiveEventCallback(std::bind(&Server<User>::receiveEvent, this, std::placeholders::_1));
            connection.setCloseEventCallback(std::bind(&Server<User>::closeEvent, this, std::placeholders::_1));
            connection.queueReceive();

            this->onAccept(user);
        }
    }

    void receiveEvent(tcp::Connection &connection)
    {
        try
        {
            User &user = usersFactory_.find(connection);
            this->onReceive(user);
        }
        catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
        {
            connection.close();
            connectionsFactory_.destroy(connection);
        }
    }

    void closeEvent(tcp::Connection &connection)
    {
        try
        {
            User &user = usersFactory_.find(connection);
            this->onClose(user);

            usersFactory_.destroy(user);

            connection.close();
            connectionsFactory_.destroy(connection);
        }
        catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
        {
            LOG(ERROR) << "user not found!";

            connection.close();
            connectionsFactory_.destroy(connection);
        }
    }

    core::common::Factory<tcp::Connection> connectionsFactory_;
    core::network::tcp::ConnectionsAcceptor connectionsAcceptor_;
    core::common::Factory<User> usersFactory_;
    core::transactions::Manager transactionsManager_;
    size_t maxNumberOfUsers_;
};

}
}
}

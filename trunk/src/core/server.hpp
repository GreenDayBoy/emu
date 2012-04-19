#ifndef eMU_CORE_SERVER_HPP
#define eMU_CORE_SERVER_HPP

#include <boost/lambda/lambda.hpp>
#include "tcpConnection.hpp"
#include "usersFactory.hpp"
#include "tcpConnectionsFactory.hpp"

namespace eMU {
namespace core {
namespace network {

template<typename UserImpl,
         typename ConnectionImpl = tcp::connection_t<>,
         typename IoServiceImpl = boost::asio::io_service,
         typename AcceptorImpl = boost::asio::ip::tcp::acceptor>
class server_t: private boost::noncopyable {
public:
    server_t(IoServiceImpl &ioService,
             uint16 port,
             size_t maxNumOfUsers):
      acceptor_(ioService,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      maxNumOfUsers_(maxNumOfUsers),
      usersFactory_(maxNumOfUsers_),
      connectionsFactory_(ioService) {}

    virtual ~server_t() {}

    void queueAccept() {
        ConnectionImpl *connection = connectionsFactory_.construct();
        connection->receiveEventCallback(boost::bind(&server_t::receiveEvent, this, _1, _2));
        connection->closeEventCallback(boost::bind(&server_t::closeEvent, this, _1));

        if(NULL == connection) {
            LOG_ERROR << "Error in allocating new connection object!" << std::endl;
            return;
        }

        acceptor_.async_accept(connection->socket(),
                               boost::bind(&server_t::acceptHandler,
                                           this,
                                           connection,
                                           boost::asio::placeholders::error));
    }

    virtual bool onAccept(UserImpl *user) = 0;
    virtual void onReceive(UserImpl *user, payload_t &payload) = 0;
    virtual void onClose(UserImpl *user) = 0;
    virtual void onStartup() = 0;
    virtual void onCleanup() = 0;

protected:
    server_t();

    void acceptHandler(ConnectionImpl *connection,
                       const boost::system::error_code &ec) {
        if(!ec) {
            this->acceptEvent(connection);
        } else {
            LOG_ERROR << "Error during establishing connection, error: " << ec.message() << std::endl;
        }

        this->queueAccept();
    }

    void acceptEvent(ConnectionImpl *connection) {
        if(usersList_.size() >= maxNumOfUsers_) {
            LOG_INFO << "Reached max number of users." << std::endl;

            connectionsFactory_.destroy(connection);
            return;
        }

        UserImpl *user = usersFactory_.construct();
        if(NULL == user) {
            LOG_ERROR << "Error in allocating new user object, address: " << connection->address() << std::endl;

            connectionsFactory_.destroy(connection);
            return;
        }

        user->connection(connection);

        if(this->onAccept(user)) {
            usersList_.push_back(user);
            connection->queueReceive();
        } else {
            connectionsFactory_.destroy(connection);
            usersFactory_.destroy(user);
        }
    }

    void receiveEvent(ConnectionImpl *connection, payload_t &payload) {
        std::vector<UserImpl*>::iterator userIter = std::find_if(usersList_.begin(), usersList_.end(), *boost::lambda::_1 == connection);

        if(usersList_.end() == userIter) {
            LOG_ERROR << "Could not find user by connection, address: " << connection->address() << std::endl;
            
            connection->disconnect();
            return;
        }

        this->onReceive(*userIter, payload);
    }

    void closeEvent(ConnectionImpl *connection) {
        std::vector<UserImpl*>::iterator userIter = std::find_if(usersList_.begin(), usersList_.end(), *boost::lambda::_1 == connection);

        if(usersList_.end() == userIter) {
            LOG_ERROR << "Could not find user by connection, address: " << connection->address() << std::endl;

            connectionsFactory_.destroy(connection);
            return;
        }

        UserImpl *user = *userIter;

        this->onClose(user);

        connectionsFactory_.destroy(connection);
        usersFactory_.destroy(user);
        usersList_.erase(userIter);
    }

    AcceptorImpl acceptor_;
    size_t maxNumOfUsers_;
    tcp::connectionsFactory_t<ConnectionImpl, IoServiceImpl> connectionsFactory_;
    user::factory_t<UserImpl> usersFactory_;
    std::vector<UserImpl*> usersList_;
};

}
}
}
#endif
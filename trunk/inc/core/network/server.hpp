#pragma once

#include <boost/lambda/lambda.hpp>
#include <network/tcp/tcpConnectionsManager.hpp>
#include <user/usersFactory.hpp>

#ifdef eMU_UT
#include <ut/env/core/ioServiceStub.hpp>
#endif

namespace eMU {
namespace core {
namespace network {

template<typename userImpl,
#ifdef eMU_UT
         typename ioServiceImpl = eMU::ut::env::core::ioServiceStub_t>
#else
         typename ioServiceImpl = boost::asio::io_service>
#endif
class server_t: private boost::noncopyable {
public:
    server_t(ioServiceImpl &ioService, uint16 port, size_t maxNumOfUsers):
      connectionsManager_(ioService, port),
      maxNumOfUsers_(maxNumOfUsers),
      usersFactory_(maxNumOfUsers_) {
        connectionsManager_.acceptEventCallback(boost::bind(&server_t::acceptEvent, this, _1));
    }
    virtual ~server_t() {}

    void queueAccept() { connectionsManager_.queueAccept(); }

    virtual void onStartup() {}
    virtual void onCleanup() {}

    virtual bool onAccept(userImpl &user) = 0;
    virtual void onReceive(userImpl &user) = 0;
    virtual void onClose(userImpl &user) = 0;

protected:
    server_t();

    void acceptEvent(tcp::connection_t<> &connection) {
        if(usersList_.size() >= maxNumOfUsers_) {
            LOG_INFO << "Reached max number of users." << std::endl;

            connectionsManager_.release(connection);
            return;
        }

        userImpl &user = usersFactory_.construct();

        user.connection(connection);
        user.connection().receiveEventCallback(boost::bind(&server_t::receiveEvent, this, _1));
        user.connection().closeEventCallback(boost::bind(&server_t::closeEvent, this, _1));

        if(this->onAccept(user)) {
            usersList_.push_back(&user);
            connection.queueReceive();
        } else {
            LOG_ERROR << "Error from onAccept() callback, address: " << connection.address() << std::endl;

            connectionsManager_.release(connection);
            usersFactory_.destroy(user);
        }
    }

    void receiveEvent(tcp::connection_t<> &connection) {
        typename std::vector<userImpl*>::iterator userIter = std::find_if(usersList_.begin(), usersList_.end(), *boost::lambda::_1 == boost::ref(connection));

        if(usersList_.end() == userIter) {
            exception_t e;
            e.in() << "Could not find user by connection, address: " << connection.address();

            connectionsManager_.release(connection);
            throw e;
        }

        userImpl *user = *userIter;
        this->onReceive(*user);
    }

    void closeEvent(tcp::connection_t<> &connection) {
        typename std::vector<userImpl*>::iterator userIter = std::find_if(usersList_.begin(), usersList_.end(), *boost::lambda::_1 == boost::ref(connection));

        if(usersList_.end() == userIter) {
            exception_t e;
            e.in() << "Could not find user by connection, address: " << connection.address();

            connectionsManager_.release(connection);
            throw e;
        }

        userImpl *user = *userIter;
        this->onClose(*user);

        connectionsManager_.release(connection);
        usersFactory_.destroy(*user);
        usersList_.erase(userIter);
    }

    tcp::connectionsManager_t<> connectionsManager_;
    size_t maxNumOfUsers_;
    user::factory_t<userImpl> usersFactory_;
    std::vector<userImpl*> usersList_;
};

}
}
}

#include <analyzer/server.hpp>
#include <analyzer/controller.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace analyzer
{

Server::Server(asio::io_service &ioService):
    core::network::Server<User>(ioService, 55555, 10) {}

bool Server::onStartup()
{
    return true;
}

void Server::onCleanup()
{
}

void Server::onAccept(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() << ", user registered.";

    controller_->onAccept(user);
}

void Server::onReceive(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() << ", received bytes: " << user.getConnection().getReadPayload().getSize();

    user.parseReadPayload();
    controller_->onReceive(user);
}

void Server::onClose(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() << ", user closed.";

    controller_->onClose(user);
}

void Server::setController(Controller *controller)
{
    controller_ = controller;
}

}
}

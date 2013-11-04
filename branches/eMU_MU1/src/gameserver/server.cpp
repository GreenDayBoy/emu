#include <gameserver/server.hpp>
#include <gameserver/messageSender.hpp>

#include <core/protocol/messagesExtractor.hpp>
#include <core/protocol/helpers.hpp>
#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace gameserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    core::network::Server<User>(core::network::tcp::ConnectionsManager::Pointer(new core::network::tcp::ConnectionsManager(ioService, configuration.port_)),
                                core::common::UsersFactory<User>::Pointer(new core::common::UsersFactory<User>(configuration.maxNumberOfUsers_)),
                                core::transactions::Manager::Pointer(new core::transactions::Manager())),
    configuration_(configuration),
    udpConnection_(new core::network::udp::Connection(ioService, configuration.port_)),
    messageSender_(std::bind(&core::network::tcp::ConnectionsManager::send, connectionsManager_, std::placeholders::_1, std::placeholders::_2))
{
    udpConnection_->setReceiveFromEventCallback(std::bind(&Server::onReceiveFrom, this, std::placeholders::_1));
}

bool Server::startup()
{
    connectionsManager_->queueAccept();
    udpConnection_->queueReceiveFrom();

    return true;
}

void Server::onAccept(size_t hash)
{
	LOG(INFO) << "hash: " << hash << ", user registered.";
}

void Server::onClose(size_t hash)
{
    LOG(INFO) << "hash: " << hash << ", user closed.";

    usersFactory_->destroy(hash);
}

void Server::onReceiveFrom(core::network::udp::Connection &connection)
{   
}

void Server::handleMessage(size_t hash, const core::network::Payload &payload)
{
}

void Server::cleanup()
{

}

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[])
{
    if(argsCount < 4)
    {
        LOG(ERROR) << "Invalid command line to start GameServer instance.";
        return 1;
    }

    google::InitGoogleLogging(args[0]);

    eMU::gameserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = boost::lexical_cast<size_t>(args[1]);
    configuration.port_ = boost::lexical_cast<uint16_t>(args[2]);

    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);

    boost::asio::io_service service;
    eMU::gameserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::gameserver::User> serviceThreading(maxNumberOfThreads, service, server);
    serviceThreading.start();

    return 0;
}
#endif

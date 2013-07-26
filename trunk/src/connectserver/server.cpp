#include <connectserver/server.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace connectserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    Server(core::network::tcp::ConnectionsManager::Pointer(new core::network::tcp::ConnectionsManager(ioService, configuration.port_)),
           core::common::UsersFactory<User>::Pointer(new core::common::UsersFactory<User>(configuration.maxNumberOfUsers_)),
           core::network::udp::Connection::Pointer(new core::network::udp::Connection(ioService, configuration.port_)),
           configuration) {}

Server::Server(core::network::tcp::ConnectionsManager::Pointer connectionsManager,
               core::common::UsersFactory<User>::Pointer usersFactory,
               core::network::udp::Connection::Pointer udpConnection,
               const Configuration &configuration):
    connectionsManager_(connectionsManager),
    usersFactory_(usersFactory),
    udpConnection_(udpConnection),
    gameServersListContent_(configuration.gameServersListContent_),
    messageSender_(std::bind(&core::network::tcp::ConnectionsManager::send, connectionsManager_, std::placeholders::_1, std::placeholders::_2))
{
    connectionsManager_->setAcceptEventCallback(std::bind(&Server::onAccept, this, std::placeholders::_1));
    connectionsManager_->setReceiveEventCallback(std::bind(&Server::onReceive, this, std::placeholders::_1, std::placeholders::_2));
    connectionsManager_->setCloseEventCallback(std::bind(&Server::onClose, this, std::placeholders::_1));
    connectionsManager_->setGenerateConnectionHashCallback(std::bind(&Server::generateConnectionHash, this));

    udpConnection_->setReceiveFromEventCallback(std::bind(&Server::onReceiveFrom, this, std::placeholders::_1));
}

void Server::startup()
{
    try
    {
        core::common::XmlReader xmlReader;
        xmlReader.parse(gameServersListContent_, "servers");

        gameServersList_.initialize(xmlReader);
        connectionsManager_->queueAccept();
    }
    catch(core::common::Exception &exception)
    {
        LOG(ERROR) << "Caught exception during connect server startup, message: " << exception.what();
    }
}

size_t Server::generateConnectionHash()
{
    try
    {
        User &user = usersFactory_->create();

        return user.hash();
    }
    catch(eMU::core::common::Exception &exception)
    {
        throw exception;
    }
}

void Server::onAccept(size_t hash)
{
    LOG(INFO) << "User registered, hash: " << hash;
}

void Server::onReceive(size_t hash, const eMU::core::network::Payload &payload)
{
    LOG(INFO) << "User received, hash: " << hash << ", size: " << payload.size();

    transactionsManager_.dequeueAll();
}

void Server::onClose(size_t hash)
{
    LOG(INFO) << "User closed, hash: " << hash;

    usersFactory_->destroy(hash);
}

void Server::onReceiveFrom(core::network::udp::Connection &connection)
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
        LOG(ERROR) << "Invalid command line to start ConnectServer instance.";
        return 1;
    }

    google::InitGoogleLogging(args[0]);

//    size_t maxNumberOfUsers = boost::lexical_cast<size_t>(args[1]);
//    uint16_t port = boost::lexical_cast<uint16_t>(args[2]);
//    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);

//    eMU::connectserver::Server::Configuration configuration = {port, maxNumberOfUsers, ""}

    return 0;
}
#endif

#include <connectserver/server.hpp>
#include <core/protocol/helpers.hpp>
#include <interface/ids.hpp>
#include <interface/constants.hpp>
#include <connectserver/transactions/gameServersListResponseTransaction.hpp>
#include <connectserver/transactions/gameServerAddressResponseTransaction.hpp>
#include <interface/gameServerAddressRequest.hpp>
#include <connectserver/transactions/gameServerLoadIndicationTransaction.hpp>
#include <interface/gameServerLoadIndication.hpp>

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
        //LOG(ERROR) << "Caught exception during connect server startup, message: " << exception.what();
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
    LOG(INFO) << "hash: " << hash << ", user registered.";
}

void Server::onReceive(size_t hash, const eMU::core::network::Payload &payload)
{
    if(!core::protocol::hasValidHeader(payload))
    {
        LOG(ERROR) << "hash: " << hash << ", received corrupted packet!";
        connectionsManager_->disconnect(hash);
        return;
    }

    if(!parse(hash, payload))
    {
        LOG(ERROR) << "hash: " << hash << ", received unknown packet!";
        connectionsManager_->disconnect(hash);
        return;
    }

    transactionsManager_.dequeueAll();
}

void Server::onClose(size_t hash)
{
    LOG(INFO) << "hash: " << hash << ", user closed.";

    usersFactory_->destroy(hash);
}

void Server::onReceiveFrom(core::network::udp::Connection &connection)
{
    if(!core::protocol::hasValidHeader(connection.readBuffer().payload_))
    {
        LOG(ERROR) << "Received corrupted packet!";
        return;
    }

    if(!parse(0, connection.readBuffer().payload_))
    {
        LOG(ERROR) << "Received unknown packet!";
        return;
    }

    transactionsManager_.dequeueAll();
}

bool Server::parse(size_t hash, const core::network::Payload &payload)
{
    uint8_t protocolId = core::protocol::getProtocolId(payload);
    uint8_t messageId = payload[4];
    bool result = true;

    if(protocolId == interface::ProtocolId::CONNECT_SERVER_PROTOCOL)
    {
        if(messageId == interface::MessageId::GAME_SERVERS_LIST_RESPONSE)
        {
            transactionsManager_.queue(new transactions::GameServersListResponseTransaction(hash, gameServersList_.list(), messageSender_));
        }
        else if(messageId == interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE)
        {
            const interface::GameServerAddressRequest *request = reinterpret_cast<const interface::GameServerAddressRequest*>(&payload[0]);
            transactionsManager_.queue(new transactions::GameServerAddressResponseTransaction(hash, messageSender_, gameServersList_, request->serverCode_));
        }
        else if(messageId == interface::MessageId::GAME_SERVER_LOAD_INDICATION)
        {
            const interface::GameServerLoadIndication *message = reinterpret_cast<const interface::GameServerLoadIndication*>(&payload[0]);
            transactionsManager_.queue(new transactions::GameServerLoadIndicationTransaction(*message, gameServersList_));
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    if(!result)
    {
        LOG(ERROR) << "hash: " << hash << ", received unknown packet!";
    }

    return result;
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

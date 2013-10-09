#include <connectserver/server.hpp>
#include <connectserver/exceptions.hpp>
#include <connectserver/transactions/gameServersListResponseTransaction.hpp>
#include <connectserver/transactions/gameServerAddressResponseTransaction.hpp>
#include <connectserver/transactions/gameServerLoadIndicationTransaction.hpp>

#include <interface/protocolIds.hpp>
#include <interface/messageIds.hpp>
#include <interface/constants.hpp>
#include <interface/gameServerAddressRequest.hpp>
#include <interface/gameServerLoadIndication.hpp>

#include <core/protocol/messagesExtractor.hpp>
#include <core/protocol/exceptions.hpp>
#include <core/protocol/helpers.hpp>
#include <core/common/exceptions.hpp>
#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace eMU
{
namespace connectserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    Server(core::network::tcp::ConnectionsManager::Pointer(new core::network::tcp::ConnectionsManager(ioService, configuration.port_)),
           core::common::UsersFactory<User>::Pointer(new core::common::UsersFactory<User>(configuration.maxNumberOfUsers_)),
           core::transactions::Manager::Pointer(new core::transactions::Manager()),
           core::network::udp::Connection::Pointer(new core::network::udp::Connection(ioService, configuration.port_)),
           configuration) {}

Server::Server(core::network::tcp::ConnectionsManager::Pointer connectionsManager,
               core::common::UsersFactory<User>::Pointer usersFactory,
               core::transactions::Manager::Pointer transactionsManager,
               core::network::udp::Connection::Pointer udpConnection,
               const Configuration &configuration):
    core::network::Server<User>(connectionsManager,
                                usersFactory,
                                transactionsManager),
    udpConnection_(udpConnection),
    messageSender_(std::bind(&core::network::tcp::ConnectionsManager::send, connectionsManager_, std::placeholders::_1, std::placeholders::_2)),
    gameServersListContent_(configuration.gameServersListContent_)
{
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
        udpConnection_->queueReceiveFrom();
    }
    catch(core::common::exceptions::EmptyXmlContentException&)
    {
        LOG(ERROR) << "Got empty xml servers list file!";
    }
    catch(core::common::exceptions::NotMatchedXmlNodeException&)
    {
        LOG(ERROR) << "Got corrupted xml servers list file!";
    }
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
    try
    {
        core::network::Payload payload(connection.readBuffer().payload_.begin(), connection.readBuffer().payload_.begin() + connection.readBuffer().payloadSize_);

        core::protocol::MessagesExtractor messageExtractor(payload);
        messageExtractor.extract();

        const core::protocol::MessagesExtractor::MessagesContainer &messages = messageExtractor.messages();

        for(const auto &message : messages)
        {
            handleMessage(0, message);
            transactionsManager_->dequeueAll();
        }
    }
    catch(core::protocol::exceptions::EmptyPayloadException&)
    {
        LOG(ERROR) << "udp, received empty payload!";
    }
    catch(core::protocol::exceptions::InvalidMessageHeaderException&)
    {
        LOG(ERROR) << "udp, invalid message header detected!";
    }
    catch(core::protocol::exceptions::InvalidMessageSizeException&)
    {
        LOG(ERROR) << "udp, invalid message size detected!";
    }
    catch(exceptions::InvalidProtocolIdException&)
    {
        LOG(ERROR) << "udp, invalid protocol id!";
    }
    catch(exceptions::UnknownMessageException&)
    {
        LOG(ERROR) << "udp, unknown message!";
    }
}

void Server::handleMessage(size_t hash, const core::network::Payload &payload)
{
    uint8_t protocolId = core::protocol::getProtocolId(payload);

    if(protocolId != interface::ProtocolId::CONNECT_SERVER_PROTOCOL)
    {
        throw exceptions::InvalidProtocolIdException();
    }

    uint8_t messageId = payload[3];

    if(messageId == interface::MessageId::GAME_SERVERS_LIST_RESPONSE)
    {
        transactionsManager_->queue(new transactions::GameServersListResponseTransaction(hash, gameServersList_.servers(), messageSender_));
    }
    else if(messageId == interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE)
    {
        const interface::GameServerAddressRequest *message = reinterpret_cast<const interface::GameServerAddressRequest*>(&payload[0]);
        transactionsManager_->queue(new transactions::GameServerAddressResponseTransaction(hash, messageSender_, gameServersList_, message->serverCode_));
    }
    else if(messageId == interface::MessageId::GAME_SERVER_LOAD_INDICATION)
    {
        const interface::GameServerLoadIndication *message = reinterpret_cast<const interface::GameServerLoadIndication*>(&payload[0]);
        transactionsManager_->queue(new transactions::GameServerLoadIndicationTransaction(*message, gameServersList_));
    }
    else
    {
        throw exceptions::UnknownMessageException();
    }
}

void Server::cleanup()
{

}

}
}

#ifdef eMU_TARGET
std::string getServersListContentFromFile(const std::string &fileName)
{
    std::ifstream file(fileName);

    if(file.is_open())
    {
        std::stringstream content;
        content << file.rdbuf();
        file.close();

        return content.str();
    }

    return "";
}

int main(int argsCount, char *args[])
{
    if(argsCount < 4)
    {
        LOG(ERROR) << "Invalid command line to start ConnectServer instance.";
        return 1;
    }

    google::InitGoogleLogging(args[0]);

    eMU::connectserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = boost::lexical_cast<size_t>(args[1]);
    configuration.port_ = boost::lexical_cast<uint16_t>(args[2]);
    configuration.gameServersListContent_ = getServersListContentFromFile("data/gameServersList.xml");

    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);

    boost::asio::io_service service;
    eMU::connectserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::connectserver::User> serviceThreading(maxNumberOfThreads, service, server);
    serviceThreading.start();

    return 0;
}
#endif

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

#include <core/protocol/packetsExtractor.hpp>
#include <core/protocol/exceptions.hpp>
#include <core/protocol/helpers.hpp>
#include <core/common/exceptions.hpp>

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

size_t Server::generateConnectionHash()
{
    try
    {
        User &user = usersFactory_->create();

        return user.hash();
    }
    catch(eMU::core::common::exceptions::MaxNumberOfUsersReachedException&)
    {
        LOG(WARNING) << "Max number of users reached.";
        return 0;
    }
}

void Server::onAccept(size_t hash)
{
    LOG(INFO) << "hash: " << hash << ", user registered.";
}

void Server::onReceive(size_t hash, const eMU::core::network::Payload &payload)
{
    try
    {
        core::protocol::PacketsExtractor packetsExtractor(payload);
        packetsExtractor.extract();

        const core::protocol::PacketsExtractor::PayloadsContainer &payloads = packetsExtractor.payloads();

        for(const auto &packet : payloads)
        {
            handleMessage(hash, packet);
            transactionsManager_.dequeueAll();
        }
    }
    catch(core::protocol::exceptions::EmptyPayloadException&)
    {
        LOG(ERROR) << "hash: " << hash << ", received empty payload!";
        connectionsManager_->disconnect(hash);
    }
    catch(core::protocol::exceptions::InvalidPacketHeaderException&)
    {
        LOG(ERROR) << "hash: " << hash << ", invalid packet header detected!";
        connectionsManager_->disconnect(hash);
    }
    catch(core::protocol::exceptions::InvalidPacketSizeException&)
    {
        LOG(ERROR) << "hash: " << hash << ", invalid packet size detected!";
        connectionsManager_->disconnect(hash);
    }
    catch(exceptions::InvalidProtocolIdException&)
    {
        LOG(ERROR) << "hash: " << hash << ", invalid protocol id!";
        connectionsManager_->disconnect(hash);
    }
    catch(exceptions::UnknownMessageException&)
    {
        LOG(ERROR) << "hash: " << hash << ", unknown message!";
        connectionsManager_->disconnect(hash);
    }
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

        core::protocol::PacketsExtractor packetExtractor(payload);
        packetExtractor.extract();

        const core::protocol::PacketsExtractor::PayloadsContainer &payloads = packetExtractor.payloads();

        for(const auto &packet : payloads)
        {
            handleMessage(0, packet);
            transactionsManager_.dequeueAll();
        }
    }
    catch(core::protocol::exceptions::EmptyPayloadException&)
    {
        LOG(ERROR) << "udp, received empty payload!";
    }
    catch(core::protocol::exceptions::InvalidPacketHeaderException&)
    {
        LOG(ERROR) << "udp, invalid packet header detected!";
    }
    catch(core::protocol::exceptions::InvalidPacketSizeException&)
    {
        LOG(ERROR) << "udp, invalid packet size detected!";
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
        transactionsManager_.queue(new transactions::GameServersListResponseTransaction(hash, gameServersList_.servers(), messageSender_));
    }
    else if(messageId == interface::MessageId::GAME_SERVER_ADDRESS_RESPONSE)
    {
        const interface::GameServerAddressRequest *message = reinterpret_cast<const interface::GameServerAddressRequest*>(&payload[0]);
        transactionsManager_.queue(new transactions::GameServerAddressResponseTransaction(hash, messageSender_, gameServersList_, message->serverCode_));
    }
    else if(messageId == interface::MessageId::GAME_SERVER_LOAD_INDICATION)
    {
        const interface::GameServerLoadIndication *message = reinterpret_cast<const interface::GameServerLoadIndication*>(&payload[0]);
        transactionsManager_.queue(new transactions::GameServerLoadIndicationTransaction(*message, gameServersList_));
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

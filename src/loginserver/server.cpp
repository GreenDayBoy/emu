#include <loginserver/server.hpp>
#include <loginserver/transactions/loginRequestTransaction.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/decoders/loginRequest.hpp>
#include <protocol/dataserver/messageIds.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace loginserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    core::network::Server<User>(ioService, configuration.port_, configuration.maxNumberOfUsers_),
    dataserverConnection_(ioService)
{
    dataserverConnection_.setConnectEventCallback(std::bind(&Server::onDataserverConnect, this, std::placeholders::_1));
    dataserverConnection_.setReceiveEventCallback(std::bind(&Server::onDataserverReceive, this, std::placeholders::_1));
    dataserverConnection_.setCloseEventCallback(std::bind(&Server::onDataserverClose, this, std::placeholders::_1));
}

bool Server::onStartup()
{
    return true;
}

void Server::onCleanup()
{
}

void Server::onAccept(size_t hash)
{
	LOG(INFO) << "hash: " << hash << ", user registered.";
}

void Server::onReceive(size_t hash, const core::network::Payload &payload)
{
    try
    {
        protocol::ReadStreamsExtractor readStreamsExtractor(payload);
        readStreamsExtractor.extract();

        for(const auto& stream : readStreamsExtractor.getStreams())
        {
            this->handleReadStream(hash, stream);

            transactionsManager_.dequeueAll();
        }
    }
    catch(const protocol::ReadStreamsExtractor::EmptyPayloadException&)
    {
        LOG(ERROR) << "Received empty payload! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
    catch(const protocol::ReadStreamsExtractor::EmptyStreamException&)
    {
        LOG(ERROR) << "Received empty stream! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
    catch(const protocol::ReadStreamsExtractor::UnknownStreamFormatException&)
    {
        LOG(ERROR) << "Received stream with unknown format! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
    catch(const protocol::ReadStream::OverflowException&)
    {
        LOG(ERROR) << "Overflow during stream read! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
    catch(const protocol::WriteStream::OverflowException&)
    {
        LOG(ERROR) << "Overflow during stream creation! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
    catch(const core::network::Payload::SizeOutOfBoundException&)
    {
        LOG(ERROR) << "Set size for payload is out of bound! hash: " << hash;
        connectionsManager_.disconnect(hash);
    }
}

void Server::onClose(size_t hash)
{
    LOG(INFO) << "hash: " << hash << ", user closed.";
    usersFactory_.destroy(hash);
}

void Server::handleReadStream(size_t hash, const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    if(messageId == protocol::loginserver::MessageIds::kLoginRequest)
    {
        protocol::loginserver::decoders::LoginRequest request(stream);
        transactionsManager_.queue(new transactions::LoginRequestTransaction(hash,
                                                                             connectionsManager_,
                                                                             dataserverConnection_,
                                                                             request));
    }
}

void Server::onDataserverConnect(core::network::tcp::Connection &connection)
{

}

void Server::onDataserverReceive(core::network::tcp::Connection &connection)
{
    try
    {
        protocol::ReadStreamsExtractor readStreamsExtractor(connection.getReadPayload());
        readStreamsExtractor.extract();

        for(const auto& stream : readStreamsExtractor.getStreams())
        {
            this->handleDataserverReadStream(stream);

            transactionsManager_.dequeueAll();
        }
    }
    catch(const protocol::ReadStreamsExtractor::EmptyPayloadException&)
    {
        LOG(ERROR) << "Dataserver, received empty payload!";
    }
    catch(const protocol::ReadStreamsExtractor::EmptyStreamException&)
    {
        LOG(ERROR) << "Dataserver, received empty stream!";
    }
    catch(const protocol::ReadStreamsExtractor::UnknownStreamFormatException&)
    {
        LOG(ERROR) << "Dataserver, received stream with unknown format!";
    }
    catch(const protocol::ReadStream::OverflowException&)
    {
        LOG(ERROR) << "Dataserver, overflow during stream read!";
    }
    catch(const protocol::WriteStream::OverflowException&)
    {
        LOG(ERROR) << "Dataserver, overflow during stream creation!";
    }
    catch(const core::network::Payload::SizeOutOfBoundException&)
    {
        LOG(ERROR) << "Dataserver, set size for payload is out of bound!";
    }
}

void Server::onDataserverClose(core::network::tcp::Connection &connection)
{

}

void Server::handleDataserverReadStream(const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    if(messageId == protocol::dataserver::MessageIds::kCheckAccountResponse)
    {

    }
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

    eMU::dataserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = boost::lexical_cast<size_t>(args[1]);
    configuration.port_ = boost::lexical_cast<uint16_t>(args[2]);

    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);

    boost::asio::io_service service;
    eMU::dataserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::dataserver::User> serviceThreading(maxNumberOfThreads, service, server);
    serviceThreading.start();

    return 0;
}
#endif

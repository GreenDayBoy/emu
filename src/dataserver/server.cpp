#include <dataserver/server.hpp>
#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace dataserver
{

Server::Server(asio::io_service &ioService, database::SqlInterface &sqlInterface, const Configuration &configuration):
    core::network::Server<User>(ioService, configuration.port_, configuration.maxNumberOfUsers_),
    sqlInterface_(sqlInterface) {}

bool Server::onStartup()
{
    return true;
}

void Server::onCleanup()
{
    sqlInterface_.cleanup();
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

    if(messageId == protocol::dataserver::MessageIds::kCheckAccountRequest)
    {
        protocol::dataserver::decoders::CheckAccountRequest request(stream);
        transactionsManager_.queue(new transactions::CheckAccountRequestTransaction(hash,
                                                                                    sqlInterface_,
                                                                                    connectionsManager_,
                                                                                    request));
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

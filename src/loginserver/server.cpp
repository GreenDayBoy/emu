#include <loginserver/server.hpp>
#include <loginserver/transactions/loginRequestTransaction.hpp>
#include <loginserver/transactions/checkAccountResponseTransaction.hpp>
#include <loginserver/transactions/faultIndicationTransaction.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/decoders/loginRequest.hpp>
#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/decoders/checkAccountResponse.hpp>
#include <protocol/dataserver/decoders/faultIndication.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/lexical_cast.hpp>

#ifdef eMU_TARGET
DEFINE_string(dataserver1_host, "127.0.0.1", "Master dataserver address");
DEFINE_int32(dataserver1_port, 55960, "Master dataserver port");
DEFINE_string(dataserver2_host, "127.0.0.1", "Slave dataserver address");
DEFINE_int32(dataserver2_port, 55962, "Slave dataserver port");
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 55557, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");
#endif

namespace eMU
{
namespace loginserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    core::network::Server<User>(ioService, configuration.port_, configuration.maxNumberOfUsers_),
    dataserverConnection_(ioService),
    configuration_(configuration),
    dataserverConnector_(dataserverConnection_, {{configuration_.dataserver1Address_, configuration_.dataserver1Port_},
                                                 {configuration_.dataserver2Address_, configuration_.dataserver2Port_}})
{
    dataserverConnection_.setReceiveEventCallback(std::bind(&Server::onDataserverReceive, this, std::placeholders::_1));
    dataserverConnection_.setCloseEventCallback(std::bind(&Server::onDataserverClose, this, std::placeholders::_1));
}

bool Server::onStartup()
{
    return dataserverConnector_.connect();
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

    LOG(INFO) << "hash: " << hash << ", received stream, id: " << messageId;

    if(messageId == protocol::loginserver::MessageIds::kLoginRequest)
    {
        User &user = usersFactory_.find(hash);

        protocol::loginserver::decoders::LoginRequest request(stream);
        transactionsManager_.queue(new transactions::LoginRequestTransaction(user,
                                                                             connectionsManager_,
                                                                             dataserverConnection_,
                                                                             request));
    }
    else if(messageId == protocol::loginserver::MessageIds::kGameServersListRequest)
    {
    }
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

    LOG(INFO) << "Dataserver, received stream, id: " << messageId;

    if(messageId == protocol::dataserver::MessageIds::kCheckAccountResponse)
    {
        protocol::dataserver::decoders::CheckAccountResponse response(stream);
        transactionsManager_.queue(new transactions::CheckAccountResponseTransaction(connectionsManager_, usersFactory_, response));
    }
    if(messageId == protocol::dataserver::MessageIds::kFaultIndication)
    {
        protocol::dataserver::decoders::FaultIndication indication(stream);
        transactionsManager_.queue(new transactions::FaultIndicationTransaction(connectionsManager_, usersFactory_, indication));
    }
}

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[])
{
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    eMU::loginserver::Server::Configuration configuration = {0};
    configuration.dataserver1Address_ = FLAGS_dataserver1_host;
    configuration.dataserver1Port_ = FLAGS_dataserver1_port;
    configuration.dataserver2Address_ = FLAGS_dataserver2_host;
    configuration.dataserver2Port_ = FLAGS_dataserver2_port;
    configuration.maxNumberOfUsers_ = FLAGS_max_users;
    configuration.port_ = FLAGS_port;

    boost::asio::io_service service;
    eMU::loginserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::loginserver::User> serviceThreading(FLAGS_max_threads, service, server);
    serviceThreading.start();

    return 0;
}
#endif

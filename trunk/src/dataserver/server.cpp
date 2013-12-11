#include <dataserver/server.hpp>
#include <dataserver/database/mySqlInterface.hpp>
#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountRequest.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/lexical_cast.hpp>

#ifdef eMU_TARGET
DEFINE_string(db_host, "127.0.0.1", "Database engine address");
DEFINE_int32(db_port, 3306, "Database engine listen port");
DEFINE_string(db_name, "mu2", "Database name");
DEFINE_string(db_user, "root", "Database engine user name");
DEFINE_string(db_password, "root", "Database engine user password");
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 44405, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");
#endif

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

    if(messageId == protocol::dataserver::MessageIds::kCheckAccountRequest)
    {
        protocol::dataserver::CheckAccountRequest request(stream);
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
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    eMU::dataserver::database::MySqlInterface mysqlInterface;
    if(!mysqlInterface.initialize())
    {
        LOG(ERROR) << "Initialization of database engine failed.";
        return 1;
    }

    if(!mysqlInterface.connect(FLAGS_db_host, FLAGS_db_port, FLAGS_db_user, FLAGS_db_password, FLAGS_db_name))
    {
        LOG(ERROR) << "Connect to database engine failed.";
        return 1;
    }

    eMU::dataserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = FLAGS_max_users;
    configuration.port_ = FLAGS_port;

    boost::asio::io_service service;
    eMU::dataserver::Server server(service, mysqlInterface, configuration);
    eMU::core::common::ServiceThreading<eMU::dataserver::User> serviceThreading(FLAGS_max_threads, service, server);
    serviceThreading.start();

    mysqlInterface.cleanup();

    return 0;
}
#endif

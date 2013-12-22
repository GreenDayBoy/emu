#include <gameserver/server.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <gflags/gflags.h>

#ifdef eMU_TARGET
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 44405, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");
#endif

namespace eMU
{
namespace gameserver
{

Server::Server(asio::io_service &ioService, const Configuration &configuration):
    core::network::Server<User>(ioService, configuration.port_, configuration.maxNumberOfUsers_),
    loginserverConnection_(ioService, configuration.port_)
{
    loginserverConnection_.setReceiveFromEventCallback(std::bind(&Server::onLoginserverReceive, this, std::placeholders::_1));
    loginserverConnection_.queueReceiveFrom();
}

bool Server::onStartup()
{
    return true;
}

void Server::onCleanup()
{
}

void Server::onAccept(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() <<  ", user registered.";
}

void Server::onReceive(User &user)
{
    try
    {
        protocol::ReadStreamsExtractor readStreamsExtractor(user.getConnection().getReadPayload());
        readStreamsExtractor.extract();

        for(const auto& stream : readStreamsExtractor.getStreams())
        {
            this->handleReadStream(user, stream);

            transactionsManager_.dequeueAll();
        }
    }
    catch(const protocol::ReadStreamsExtractor::EmptyPayloadException&)
    {
        LOG(ERROR) << "Received empty payload! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
    catch(const protocol::ReadStreamsExtractor::EmptyStreamException&)
    {
        LOG(ERROR) << "Received empty stream! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
    catch(const protocol::ReadStreamsExtractor::UnknownStreamFormatException&)
    {
        LOG(ERROR) << "Received stream with unknown format! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
    catch(const protocol::ReadStream::OverflowException&)
    {
        LOG(ERROR) << "Overflow during stream read! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
    catch(const protocol::WriteStream::OverflowException&)
    {
        LOG(ERROR) << "Overflow during stream creation! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
    catch(const core::network::Payload::SizeOutOfBoundException&)
    {
        LOG(ERROR) << "Set size for payload is out of bound! hash: " << user.getHash();
        user.getConnection().disconnect();
    }
}

void Server::onClose(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() << " closed.";
}

void Server::handleReadStream(User &user, const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    LOG(INFO) << "hash: " << user.getHash() << ", received stream, id: " << messageId;
}

void Server::onLoginserverReceive(core::network::udp::Connection &connection)
{

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

    eMU::gameserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = FLAGS_max_users;
    configuration.port_ = FLAGS_port;

    boost::asio::io_service service;
    eMU::gameserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::gameserver::User> serviceThreading(FLAGS_max_threads, service, server);
    serviceThreading.start();
    serviceThreading.join();

    return 0;
}
#endif

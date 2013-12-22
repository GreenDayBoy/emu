#include <loginserver/server.hpp>
#include <loginserver/transactions/loginRequest.hpp>
#include <loginserver/transactions/checkAccountResponse.hpp>
#include <loginserver/transactions/faultIndication.hpp>
#include <loginserver/transactions/gameserversListRequest.hpp>
#include <loginserver/transactions/gameserverDetailsRequest.hpp>

#include <protocol/readStreamsExtractor.hpp>
#include <protocol/writeStream.hpp>

#include <protocol/loginserver/messageIds.hpp>
#include <protocol/loginserver/loginRequest.hpp>
#include <protocol/loginserver/gameserversListRequest.hpp>
#include <protocol/loginserver/gameserverDetailsRequest.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/faultIndication.hpp>

#include <core/common/serviceThreading.hpp>

#include <glog/logging.h>
#include <gflags/gflags.h>

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
    bool succeed = false;

    try
    {
        core::common::XmlReader xmlReader;
        xmlReader.parse(configuration_.gameserversListFileContent_, "servers");
        gameserversList_.initialize(xmlReader);

        succeed = dataserverConnector_.connect();
    }
    catch(core::common::XmlReader::EmptyXmlContentException&)
    {
        LOG(ERROR) << "Got empty xml servers list file!";
    }
    catch(core::common::XmlReader::NotMatchedXmlNodeException&)
    {
        LOG(ERROR) << "Got corrupted xml servers list file!";
    }

    return succeed;
}

void Server::onCleanup()
{
}

void Server::onAccept(User &user)
{
    LOG(INFO) << "hash: " << user.getHash() << ", user registered.";
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
    LOG(INFO) << "hash: " << user.getHash() << ", user closed.";
}

void Server::handleReadStream(User &user, const protocol::ReadStream &stream)
{
    uint16_t messageId = stream.getId();

    LOG(INFO) << "hash: " << user.getHash() << ", received stream, id: " << messageId;

    if(messageId == protocol::loginserver::MessageIds::kLoginRequest)
    {
        protocol::loginserver::LoginRequest request(stream);
        transactionsManager_.queue(new transactions::LoginRequest(user, dataserverConnection_, request));
    }
    else if(messageId == protocol::loginserver::MessageIds::kGameserversListRequest)
    {
        protocol::loginserver::GameserversListRequest request(stream);
        transactionsManager_.queue((new transactions::GameserversListRequest(user, gameserversList_, request)));
    }
    else if(messageId == protocol::loginserver::MessageIds::kGameserverDetailsRequest)
    {
        protocol::loginserver::GameserverDetailsRequest request(stream);
        transactionsManager_.queue((new transactions::GameserverDetailsRequest(user, gameserversList_, request)));
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
        protocol::dataserver::CheckAccountResponse response(stream);
        transactionsManager_.queue(new transactions::CheckAccountResponse(usersFactory_, response));
    }
    if(messageId == protocol::dataserver::MessageIds::kFaultIndication)
    {
        protocol::dataserver::FaultIndication indication(stream);
        transactionsManager_.queue(new transactions::FaultIndication(usersFactory_, indication));
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

    eMU::loginserver::Server::Configuration configuration = {0};
    configuration.dataserver1Address_ = FLAGS_dataserver1_host;
    configuration.dataserver1Port_ = FLAGS_dataserver1_port;
    configuration.dataserver2Address_ = FLAGS_dataserver2_host;
    configuration.dataserver2Port_ = FLAGS_dataserver2_port;
    configuration.maxNumberOfUsers_ = FLAGS_max_users;
    configuration.port_ = FLAGS_port;
    configuration.gameserversListFileContent_ = eMU::core::common::XmlReader::getXmlFileContent("./data/gameserversList.xml");

    boost::asio::io_service service;
    eMU::loginserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::loginserver::User> serviceThreading(FLAGS_max_threads, service, server);
    serviceThreading.start();
    serviceThreading.join();

    return 0;
}
#endif

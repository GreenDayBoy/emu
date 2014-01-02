#include <loginserver/main.hpp>
#include <loginserver/context.hpp>
#include <loginserver/protocol.hpp>
#include <loginserver/dataserverProtocol.hpp>
#include <loginserver/gameserverProtocol.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>
#include <core/common/xmlReader.hpp>
#include <core/common/concurrency.hpp>
#include <core/network/udp/connection.hpp>

#include <boost/thread.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(dataserver_host, "127.0.0.1", "Dataserver address");
DEFINE_int32(dataserver_port, 55960, "Dataserver port");
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 55557, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");


int main(int argsCount, char *args[])
{
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    eMU::loginserver::Context loginserverContext(FLAGS_max_users);
    eMU::core::common::XmlReader xmlReader(eMU::core::common::XmlReader::getXmlFileContent("./data/gameserversList.xml"));

    if(!loginserverContext.getGameserversList().initialize(xmlReader))
    {
        LOG(ERROR) << "Unable to parse gameservers list!";
        return 1;
    }

    eMU::loginserver::DataserverProtocol dataserverProtocol(loginserverContext);

    boost::asio::io_service ioService;
    eMU::core::network::tcp::Connection::Pointer dataserverConnection(new eMU::core::network::tcp::Connection(ioService, dataserverProtocol));
    if(!dataserverConnection->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(FLAGS_dataserver_host),
                                                                    FLAGS_dataserver_port)))
    {
        LOG(ERROR) << "Connection to datserver failed. Address: " << FLAGS_dataserver_host << ", port: " << FLAGS_dataserver_port;
        return 1;
    }

    eMU::loginserver::GameserverProtocol gameserverProtocol(loginserverContext);
    eMU::core::network::udp::Connection gameserverConnection(ioService, FLAGS_port, gameserverProtocol);
    gameserverConnection.queueReceiveFrom();

    eMU::loginserver::Protocol loginserverProtocol(loginserverContext);
    eMU::core::network::tcp::ConnectionsAcceptor connectionsAcceptor(ioService, FLAGS_port, loginserverProtocol);
    connectionsAcceptor.queueAccept();

    eMU::core::common::Concurrency concurrency(ioService, FLAGS_max_threads);
    concurrency.start();
    concurrency.join();

    return 0;
}

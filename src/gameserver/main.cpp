#include <gameserver/main.hpp>
#include <gameserver/context.hpp>
#include <gameserver/protocol.hpp>
#include <gameserver/dataserverProtocol.hpp>
#include <gameserver/udpProtocol.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>
#include <core/common/concurrency.hpp>
#include <core/network/tcp/connection.hpp>

#include <boost/thread.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(dataserver_host, "127.0.0.1", "Dataserver address");
DEFINE_int32(dataserver_port, 55960, "Dataserver port");
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 55901, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");
DEFINE_int32(code, 0, "gameserver code");


int main(int argsCount, char *args[])
{
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    eMU::gameserver::Context gameserverContext(FLAGS_max_users, FLAGS_code);

    eMU::gameserver::DataserverProtocol dataserverProtocol(gameserverContext);

    boost::asio::io_service ioService;
    eMU::core::network::tcp::Connection::Pointer dataserverConnection(new eMU::core::network::tcp::Connection(ioService, dataserverProtocol));
    if(!dataserverConnection->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(FLAGS_dataserver_host),
                                                                    FLAGS_dataserver_port)))
    {
        LOG(ERROR) << "Connection to datserver failed. host: " << FLAGS_dataserver_host << ", port: " << FLAGS_dataserver_port;
        return 1;
    }

    eMU::gameserver::UdpProtocol udpProtocol(gameserverContext);
    eMU::core::network::udp::Connection::Pointer udpConnection(new eMU::core::network::udp::Connection(ioService, FLAGS_port, udpProtocol));
    udpConnection->registerConnection();
    udpConnection->queueReceiveFrom();

    eMU::gameserver::Protocol gameserverProtocol(gameserverContext);
    eMU::core::network::tcp::ConnectionsAcceptor connectionsAcceptor(ioService, FLAGS_port, gameserverProtocol);
    connectionsAcceptor.queueAccept();

    eMU::core::common::Concurrency concurrency(ioService, FLAGS_max_threads);
    concurrency.start();
    concurrency.join();

    udpConnection->unregisterConnection();

    return 0;
}

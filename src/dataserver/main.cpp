#include <dataserver/main.hpp>
#include <dataserver/context.hpp>
#include <dataserver/protocol.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>

#include <boost/thread.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(db_host, "127.0.0.1", "Database engine address");
DEFINE_int32(db_port, 3306, "Database engine listen port");
DEFINE_string(db_name, "mu2", "Database name");
DEFINE_string(db_user, "root", "Database engine user name");
DEFINE_string(db_password, "root", "Database engine user password");
DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 55960, "server listen port");
DEFINE_int32(max_threads, 2, "max number of concurrent threads");

int main(int argsCount, char *args[])
{
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    eMU::dataserver::Context dataserverContext(FLAGS_max_users);
    if(!dataserverContext.getSqlInterface().initialize())
    {
        LOG(ERROR) << "Initialization of database engine failed.";
        return 1;
    }

    if(!dataserverContext.getSqlInterface().connect(FLAGS_db_host, FLAGS_db_port, FLAGS_db_user, FLAGS_db_password, FLAGS_db_name))
    {
        LOG(ERROR) << "Connect to database engine failed.";
        return 1;
    }

    eMU::dataserver::Protocol dataserverProtocol(dataserverContext);
    boost::asio::io_service ioService;

    eMU::core::network::tcp::ConnectionsAcceptor connectionsAcceptor(ioService, FLAGS_port, dataserverProtocol);
    connectionsAcceptor.queueAccept();

    boost::thread_group threads;
    for(size_t i = 0; i < FLAGS_max_threads; ++i)
    {
        threads.create_thread(std::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService));
    }

    threads.join_all();

    dataserverContext.getSqlInterface().cleanup();

    return 0;
}

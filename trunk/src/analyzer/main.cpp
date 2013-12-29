#include <analyzer/main.hpp>
#include <analyzer/controller.hpp>
#include <analyzer/protocol.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

#include <boost/thread.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_int32(max_users, 5, "Max number of users to connect");
DEFINE_int32(port, 55555, "server listen port");
DEFINE_int32(max_threads, 1, "max number of concurrent threads");

int main(int argsCount, char *args[])
{
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::ParseCommandLineFlags(&argsCount, &args, true);
    google::InitGoogleLogging(args[0]);

    QApplication analyzer(argsCount, args);

    eMU::analyzer::Controller controller(FLAGS_max_users);
    controller.getMainView().display();

    eMU::analyzer::Protocol analyzerProtocol(controller);

    boost::asio::io_service ioService;
    eMU::core::network::tcp::ConnectionsAcceptor connectionsAcceptor(ioService, FLAGS_port, analyzerProtocol);
    connectionsAcceptor.queueAccept();

    boost::thread_group threads;
    for(size_t i = 0; i < FLAGS_max_threads; ++i)
    {
        threads.create_thread(std::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService));
    }

    return analyzer.exec();
}

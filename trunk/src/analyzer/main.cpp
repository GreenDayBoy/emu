#include <analyzer/main.hpp>
#include <analyzer/controller.hpp>
#include <analyzer/protocol.hpp>
#include <core/network/tcp/connectionsAcceptor.hpp>
#include <core/common/concurrency.hpp>

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

    eMU::core::common::Concurrency concurrency(ioService, FLAGS_max_threads);
    concurrency.start();

    return analyzer.exec();
}

#include <analyzer/main.hpp>
#include <analyzer/controller.hpp>

#include <core/common/serviceThreading.hpp>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

int main(int argsCount, char *args[])
{
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(args[0]);

    QApplication analyzer(argsCount, args);

    boost::asio::io_service service;
    eMU::analyzer::Controller controller(service);
    controller.getMainView().display();

    eMU::core::common::ServiceThreading<eMU::analyzer::User> serviceThreading(1, service, controller.getServer());
    serviceThreading.start();

    return analyzer.exec();
}

#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

#include <glog/logging.h>

#include <application/serviceThreading.hpp>
#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace application {

template<typename Server>
class Starter: boost::noncopyable {
public:
    Starter(size_t maxNumberOfThreads,
            int16_t port,
            size_t maxNumberOfUsers):
      serviceThreading_(maxNumberOfThreads),
      signalSet_(serviceThreading_.ioService()),
      server_(serviceThreading_.ioService(), port, maxNumberOfUsers) {}

    void start() {
        try {
            LOG(INFO) << "Starting server.";

            signalSet_.add(SIGTERM);
            signalSet_.add(SIGINT);

            signalSet_.async_wait(std::bind(&Starter::stopHandler, this));
            server_.startup();

            serviceThreading_.initialize();
            serviceThreading_.join();
        } catch(common::Exception &e) {
            LOG(ERROR) << "eMU exception: " << e.what() << std::endl;
        } catch(std::exception &e) {
            LOG(ERROR) << "std exception: " << e.what() << std::endl;
        }
    }

    void stopHandler() {
        try {
            LOG(INFO) << "Stoping server.";

            server_.cleanup();
            serviceThreading_.stop();
        } catch(common::Exception &e) {
            LOG(ERROR) << "eMU exception: " << e.what() << std::endl;
        } catch(std::exception &e) {
            LOG(ERROR) << "std exception: " << e.what() << std::endl;
        }
    }

private:
    ServiceThreading serviceThreading_;
    boost::asio::signal_set signalSet_;
    Server server_;
};

}
}
}

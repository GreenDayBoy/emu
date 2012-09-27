#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <serviceThreading.hpp>
#include <common/log.hpp>
#include <shared/types.hpp>

namespace eMU {
namespace core {

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

template<typename serverImpl>
class baseApplication_t: private boost::noncopyable {
public:
    baseApplication_t(size_t maxNumOfThreads,
                      int16 port,
                      size_t maxNumOfUsers):
      server_(ioService_, port, maxNumOfUsers),
      signalSet_(ioService_),
      serviceThreading_(maxNumOfThreads) {}
    virtual ~baseApplication_t() {}

    void start() {
        LOG_INFO << "Starting server." << std::endl;

        signalSet_.add(SIGTERM);
        signalSet_.add(SIGINT);

        signalSet_.async_wait(boost::bind(&baseApplication_t::stopHandler, this));
        server_.onStartup();
        server_.queueAccept();

        serviceThreading_.initialize(ioService_);
        serviceThreading_.join();
    }

    void stopHandler() {
        server_.onCleanup();
        ioService_.stop();
    }

protected:
    boost::asio::io_service ioService_;
    boost::asio::signal_set signalSet_;
    serviceThreading_t serviceThreading_;
    serverImpl server_;
};

}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

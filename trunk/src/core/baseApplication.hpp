#ifndef eMU_CORE_BASEAPPLICATION_HPP
#define eMU_CORE_BASEAPPLICATION_HPP

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include "../shared/types.hpp"
#include "server.hpp"
#include "serviceThreading.hpp"

namespace eMU {
namespace core {

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

template<typename UserImpl>
class baseApplication_t: private boost::noncopyable {
public:
    baseApplication_t(boost::asio::io_service &ioService,
                      eMU::core::network::server_t<UserImpl> &server,
                      size_t maxNumOfThreads):
      ioService_(ioService),
      server_(server),
      signalSet_(ioService_),
      serviceThreading_(ioService, maxNumOfThreads) {}
    virtual ~baseApplication_t() {}

    void initialize() {
        signalSet_.add(SIGTERM);
        signalSet_.add(SIGINT);
        server_.onStartup();
    }

    void start() {
        LOG_INFO << "Starting server." << std::endl;
        signalSet_.async_wait(boost::bind(&baseApplication_t::stopHandler, this));
        serviceThreading_.join();
    }

    void stopHandler() {
        server_.onCleanup();
        ioService_.stop();
    }

protected:
    boost::asio::io_service &ioService_;
    boost::asio::signal_set signalSet_;
    serviceThreading_t serviceThreading_;
    network::server_t<UserImpl> &server_;
};

}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif
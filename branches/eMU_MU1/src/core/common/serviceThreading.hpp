#pragma once

#include <functional>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <core/network/server.hpp>

namespace eMU
{
namespace core
{
namespace common
{

template<typename User>
class ServiceThreading: private boost::noncopyable {
public:
    ServiceThreading(size_t maxNumberOfThreads,
                     boost::asio::io_service &ioService,
                     core::network::Server<User> &server):
        maxNumberOfThreads_(maxNumberOfThreads),
        ioService_(ioService),
        server_(server),
        signalSet_(ioService_) {}

    ~ServiceThreading()
    {
        threads_.interrupt_all();
    }

    void start()
    {
        signalSet_.add(SIGTERM);
        signalSet_.add(SIGINT);
        signalSet_.async_wait(std::bind(&ServiceThreading::stopHandler, this));

        if(server_.startup())
        {
            for(size_t i = 0; i < maxNumberOfThreads_; ++i)
            {
                threads_.create_thread(std::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService_));
            }

            threads_.join_all();
        }
        else
        {
            LOG(ERROR) << "Startup server failed.";
        }
    }

    void stopHandler()
    {
        server_.cleanup();
        ioService_.stop();
    }

private:
    size_t maxNumberOfThreads_;
    boost::asio::io_service &ioService_;
    core::network::Server<User> &server_;
    boost::thread_group threads_;
    boost::asio::signal_set signalSet_;
};

}
}
}

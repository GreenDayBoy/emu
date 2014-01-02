#pragma once

#include <core/common/asio.hpp>

#include <boost/thread.hpp>

namespace eMU
{
namespace core
{
namespace common
{

class Concurrency
{
public:
    Concurrency(asio::io_service &ioService, size_t maxNumberOfThreads);

    void start();
    void join();

private:
    Concurrency();

    asio::io_service &ioService_;
    size_t maxNumberOfThreads_;
    boost::thread_group threads_;
};

}
}
}

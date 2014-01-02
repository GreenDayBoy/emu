#include <core/common/concurrency.hpp>

namespace eMU
{
namespace core
{
namespace common
{

Concurrency::Concurrency(asio::io_service &ioService, size_t maxNumberOfThreads):
    ioService_(ioService),
    maxNumberOfThreads_(maxNumberOfThreads) {}

void Concurrency::start()
{
    for(size_t i = 0; i < maxNumberOfThreads_; ++i)
    {
        threads_.create_thread(std::bind(static_cast<size_t (asio::io_service::*)()>(&asio::io_service::run), &ioService_));
    }
}

void Concurrency::join()
{
    threads_.join_all();
}

}
}
}

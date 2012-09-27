#pragma once

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <core.hpp>

#ifdef WIN32
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

namespace eMU {
namespace core {

class eMU_CORE_DECLSPEC serviceThreading_t: private boost::noncopyable {
public:
    serviceThreading_t(size_t maxNumOfThreads);
    virtual ~serviceThreading_t();

    void initialize(boost::asio::io_service &ioService);
    void join();

private:
    boost::thread_group threads_;
    size_t maxNumOfThreads_;
};

}
}

#ifdef WIN32
#pragma warning(default: 4251)
#pragma warning(default: 4275)
#endif

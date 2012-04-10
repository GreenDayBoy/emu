#ifndef eMU_CORE_SERVICETHREADING_HPP
#define eMU_CORE_SERVICETHREADING_HPP

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "core.hpp"

#ifdef WIN32
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

namespace eMU {
namespace core {

class eMU_CORE_DECLSPEC serviceThreading_t: private boost::noncopyable {
public:
    serviceThreading_t(boost::asio::io_service &ioService,
                       size_t maxNumOfThreads);
    virtual ~serviceThreading_t();

    void join();

private:
    boost::thread_group threads_;
};

}
}

#ifdef WIN32
#pragma warning(default: 4251)
#pragma warning(default: 4275)
#endif

#endif
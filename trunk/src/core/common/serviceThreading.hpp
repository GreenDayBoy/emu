#pragma once

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#ifdef eMU_UT
#include <ut/env/asioStub/ioService.hpp>
#elif eMU_MT
#include <mt/env/asioStub/ioService.hpp>
#else
#include <boost/asio.hpp>
#endif

namespace eMU {
namespace core {
namespace common {

#ifdef eMU_UT
namespace asio = eMU::ut::env::asioStub;
#elif eMU_MT
namespace asio = eMU::mt::env::asioStub;
#else
namespace asio = boost::asio;
#endif

class ServiceThreading: private boost::noncopyable {
public:
    ServiceThreading(size_t maxNumberOfThreads);
    virtual ~ServiceThreading();

    void initialize();
    void join();

private:
    boost::thread_group threads_;
    size_t maxNumberOfThreads_;
    asio::io_service ioService_;
};

}
}
}

#ifndef eMU_UT_IOSERVICESTUB_HPP
#define eMU_UT_IOSERVICESTUB_HPP

#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include "../../shared/types.hpp"

namespace eMU {
namespace ut {
namespace network {

class ioServiceStub_t: private boost::noncopyable {
public:
    typedef boost::function2<void, const boost::system::error_code&, size_t> ioHandler_t;

    class strand {
    public:
        strand(ioServiceStub_t &ioService) {}

        ioHandler_t wrap(const ioHandler_t &handler) {
            return handler;
        }

    private:
        strand();
    };
};

}
}
}

#endif
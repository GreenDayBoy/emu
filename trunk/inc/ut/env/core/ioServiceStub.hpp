#pragma once

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {

class ioServiceStub_t: private boost::noncopyable {
public:
    typedef boost::function2<void, const boost::system::error_code&, size_t> ioHandler_t;

    class strand {
    public:
        strand(ioServiceStub_t &ioService) {}

        template<typename completionHandlerImpl>
        completionHandlerImpl wrap(const completionHandlerImpl &handler) { return handler; }

    private:
        strand();
    };

    template<typename completionHandlerImpl>
    void post(const completionHandlerImpl &handler) { handler(); }
};

}
}
}
}

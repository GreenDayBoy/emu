#pragma once

#include <functional>
#include <boost/system/error_code.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class io_service
{
public:
    class strand
    {
    public:
        strand(io_service& service);

        template<typename CompletionHandler>
        CompletionHandler wrap(const CompletionHandler &handler) { return handler; }
    };

    typedef std::function<void(const boost::system::error_code, size_t)> IoHandler;

    template<typename CompletionHandler>
    void post(const CompletionHandler &handler) { handler(); }

    size_t run();
    void stop();
};

}
}
}
}

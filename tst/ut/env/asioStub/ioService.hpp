#pragma once

#include <boost/system/error_code.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace asioStub
{

class io_service
{
public:
    typedef std::function<void(const boost::system::error_code, size_t)> IoHandler;

    class strand
    {
    public:
        strand(io_service& service);

        template<typename CompletionHandler>
        CompletionHandler wrap(const CompletionHandler &handler)
        {
            return handler;
        }

    };

    template<typename CompletionHandler>
    void post(const CompletionHandler &handler)
    {
        handler();
    }
    size_t run();
    void stop();
};

}
}
}
}

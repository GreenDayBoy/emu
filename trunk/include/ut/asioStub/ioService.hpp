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

        template<typename CompletionHandlerType>
        CompletionHandlerType wrap(const CompletionHandlerType &handler)
        {
            return handler;
        }

    };

    template<typename CompletionHandlerType>
    void post(const CompletionHandlerType &handler)
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

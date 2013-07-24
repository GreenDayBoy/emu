#include <ut/env/asioStub/ioService.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace asioStub
{

io_service::strand::strand(io_service& service) {}

size_t io_service::run()
{
    return 0;
}

void io_service::stop() {}

}
}
}
}

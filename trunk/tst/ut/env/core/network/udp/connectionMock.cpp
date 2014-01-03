#include <ut/env/core/network/udp/connectionMock.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace core
{
namespace network
{
namespace udp
{

ConnectionMock::ConnectionMock():
    Connection(ioService_, 12456, protocol_) {}

}
}
}
}
}
}

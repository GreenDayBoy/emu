#include <core/network/tcp/protocol.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

Protocol::~Protocol()
{

}

bool Protocol::attach(Connection::Pointer)
{
    return true;
}

void Protocol::detach(Connection::Pointer)
{

}

bool Protocol::dispatch(Connection::Pointer)
{
    return true;
}

void Protocol::shutdown()
{

}

}
}
}
}

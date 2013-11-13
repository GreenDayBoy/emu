#pragma once

#include <gmock/gmock.h>
#include <core/network/tcp/connectionsFactory.hpp>

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
namespace tcp
{

namespace network = eMU::core::network;

class ConnectionsFactoryMock: public eMU::core::network::tcp::ConnectionsFactory
{
public:
    MOCK_METHOD2(create, network::tcp::Connection&(size_t hash, network::tcp::Connection::SocketPointer socket));
    MOCK_METHOD1(destroy, void(size_t hash));
    MOCK_METHOD1(get, network::tcp::Connection&(size_t hash));
    MOCK_CONST_METHOD1(getHash, size_t(const network::tcp::Connection&));
};

}
}
}
}
}
}

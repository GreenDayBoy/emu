#pragma once

#include <core/network/payload.hpp>
#include <core/network/tcp/connectionsManager.hpp>

#include <gmock/gmock.h>

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

class ConnectionsManagerMock: public eMU::core::network::tcp::ConnectionsManager
{
public:
    ConnectionsManagerMock();

    MOCK_METHOD2(send, void(size_t hash, const eMU::core::network::Payload &payload));

private:
    asio::io_service service_;
};

}
}
}
}
}
}

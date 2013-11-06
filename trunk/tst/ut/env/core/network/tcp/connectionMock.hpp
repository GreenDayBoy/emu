#pragma once

#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>

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

class ConnectionMock: public eMU::core::network::tcp::Connection
{
public:
    ConnectionMock(Connection::SocketPointer socket);

    MOCK_METHOD0(queueReceive, void());
    MOCK_METHOD1(setReceiveEventCallback, void(const EventCallback &callback));
    MOCK_METHOD1(setCloseEventCallback, void(const EventCallback &callback));
    MOCK_METHOD1(send, void(const eMU::core::network::Payload &payload));
    MOCK_METHOD0(disconnect, void());
    MOCK_METHOD0(close, void());
};

}
}
}
}
}
}

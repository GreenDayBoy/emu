#pragma once

#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>
#include <core/network/tcp/protocol.hpp>

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
    typedef std::shared_ptr<ConnectionMock> Pointer;

    ConnectionMock();

    MOCK_METHOD0(queueReceive, void());
    MOCK_METHOD1(send, void(const eMU::core::network::Payload &payload));
    MOCK_METHOD0(disconnect, void());
    MOCK_METHOD0(close, void());
    MOCK_CONST_METHOD0(isOpen, bool());
    MOCK_METHOD1(connect, bool(const boost::asio::ip::tcp::endpoint &endpoint));

private:   
    asioStub::io_service ioService_;
    eMU::core::network::tcp::Protocol protocol_;
};

}
}
}
}
}
}

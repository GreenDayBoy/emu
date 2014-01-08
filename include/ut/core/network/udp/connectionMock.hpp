#pragma once

#include <gmock/gmock.h>
#include <core/network/udp/connection.hpp>
#include <core/network/udp/protocol.hpp>

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

class ConnectionMock: public eMU::core::network::udp::Connection
{
public:
    typedef std::shared_ptr<ConnectionMock> Pointer;

    ConnectionMock();

    MOCK_METHOD2(sendTo, void(const boost::asio::ip::udp::endpoint &endpoint, const eMU::core::network::Payload &payload));

private:   
    asioStub::io_service ioService_;
    eMU::core::network::udp::Protocol protocol_;
};

}
}
}
}
}
}

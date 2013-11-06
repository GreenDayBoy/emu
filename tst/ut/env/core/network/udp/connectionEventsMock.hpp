#pragma once

#include <gmock/gmock.h>
#include <core/network/udp/connection.hpp>

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

class ConnectionEventsMock
{
public:
    MOCK_METHOD2(receiveFromEvent, void(eMU::core::network::udp::Connection&, const boost::asio::ip::udp::endpoint&));
};

}
}
}
}
}
}

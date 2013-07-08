#pragma once

#include <gmock/gmock.h>
#include <core/network/tcp/connection.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace tcp {

class ConnectionEventsMock {
public:
    MOCK_METHOD1(connectEvent, void(eMU::core::network::tcp::Connection &connection));
    MOCK_METHOD1(receiveEvent, void(eMU::core::network::tcp::Connection &connection));
    MOCK_METHOD1(closeEvent, void(eMU::core::network::tcp::Connection &connection));
};

}
}
}
}
}

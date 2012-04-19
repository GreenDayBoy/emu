#ifndef eMU_UT_TYPES_HPP
#define eMU_UT_TYPES_HPP

#include "../../core/tcpConnection.hpp"
#include "ioServiceStub.hpp"
#include "tcpSocketMock.hpp"

namespace eMU {
namespace ut {
namespace network {
namespace tcp {

typedef eMU::core::network::tcp::connection_t<eMU::ut::network::tcp::socketMock_t,
                                              eMU::ut::network::ioServiceStub_t> testConnection_t;

}
}
}
}

#endif
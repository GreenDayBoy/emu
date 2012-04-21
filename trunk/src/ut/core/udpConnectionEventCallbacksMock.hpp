#ifndef eMU_UT_UDPCONNECTIONEVENTCALLBACKSMOCK_HPP
#define eMU_UT_UDPCONNECTIONEVENTCALLBACKSMOCK_HPP

#include <gmock/gmock.h>
#include "../../core/buffer.hpp"
#include "types.hpp"

namespace eMU {
namespace ut {
namespace network {
namespace udp {

namespace eMUNetwork = eMU::core::network;

class connectionEventCallbacksMock_t {
public:
    MOCK_METHOD3(receiveFromEvent, void(testConnection_t *connection,
                                        const boost::asio::ip::udp::endpoint &endpoint,
                                        eMUNetwork::payload_t &payload));

    void expectCall_receiveFromEvent(testConnection_t *connection,
                                     const boost::asio::ip::udp::endpoint &endpoint,
                                     eMUNetwork::payload_t &payload);
};

}
}
}
}


#endif
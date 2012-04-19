#ifndef eMU_UT_CONNECTIONEVENTCALLBACKSMOCK_HPP
#define eMU_UT_CONNECTIONEVENTCALLBACKSMOCK_HPP

#include "../../core/tcpConnection.hpp"
#include "ioServiceStub.hpp"
#include "socketMock.hpp"

namespace eMU {
namespace ut {
namespace network {

namespace eMUNetwork = eMU::core::network;

class connectionEventCallbacksMock_t {
public:
    typedef eMUNetwork::tcp::connection_t<socketMock_t,
                                          ioServiceStub_t> testConnection_t;

    MOCK_METHOD1(connectEvent, void(testConnection_t *connection));
    MOCK_METHOD2(receiveEvent, void(testConnection_t *connection,
                                    eMUNetwork::payload_t &payload));
    MOCK_METHOD1(closeEvent, void(testConnection_t *connection));

    void expectCall_connectEvent(testConnection_t *connection);
    void expectCall_receiveEvent(testConnection_t *connection,
                                 eMUNetwork::payload_t &payload);
    void expectCall_closeEvent(testConnection_t *connection);
};

}
}
}

#endif
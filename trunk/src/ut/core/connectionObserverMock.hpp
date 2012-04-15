#ifndef eMU_UT_CONNECTIONOBSERVERMOCK_HPP
#define eMU_UT_CONNECTIONOBSERVERMOCK_HPP

#include "../../core/connection.hpp"
#include "ioServiceStub.hpp"
#include "socketMock.hpp"

namespace eMU {
namespace ut {
namespace network {

namespace eMUNetwork = eMU::core::network;

class connectionObserverMock_t: public eMUNetwork::connection_t<socketMock_t, ioServiceStub_t>::observer_i {
public:
    MOCK_METHOD1(connectEvent, void(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection));
    MOCK_METHOD2(receiveEvent, void(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection,
                                    eMUNetwork::payload_t &payload));
    MOCK_METHOD1(closeEvent, void(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection));

    void expectCall_connectEvent(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection);
    void expectCall_receiveEvent(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection,
                                 eMUNetwork::payload_t &payload);
    void expectCall_closeEvent(eMUNetwork::connection_t<socketMock_t, ioServiceStub_t> *connection);
};

}
}
}

#endif
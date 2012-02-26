#ifndef eMUUT_SOCKETCALLBACKMOCK_HPP
#define eMUUT_SOCKETCALLBACKMOCK_HPP

#include <gmock/gmock.h>
#include "ioServiceMock.hpp"
#include "socketStub.hpp"
#include "../../core/socket.hpp"

namespace eMUUnitTest {
namespace networkTest {

namespace eMUNetwork = eMUCore::network;

class socketCallbackMock_t  {
public:
    MOCK_METHOD1(onReceive, void(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket));
    MOCK_METHOD1(onClose, void(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket));

    void expectCall_onClose(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket);
    void expectCall_onReceive(eMUNetwork::socket_t<ioServiceMock_t, socketStub_t>::ptr_t socket);
};

}
}

#endif
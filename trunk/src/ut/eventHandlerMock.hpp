#ifndef eMUUT_EVENTHANDLERMOCK_HPP
#define eMUUT_EVENTHANDLERMOCK_HPP

#include <gmock/gmock.h>
#include "ioServiceMock.hpp"
#include "socketStub.hpp"
#include "../core/socket.hpp"

namespace eMUUnitTest {
namespace networkTest {

class eventHandlerMock_t
    : public eMUCore::network::socket_t<eMUUnitTest::networkTest::ioServiceMock_t,
                                        eMUUnitTest::networkTest::socketStub_t>::eventHandler_t  {
public:
    MOCK_METHOD2(onReceive, void(uint8 *payload, size_t size));
    MOCK_METHOD0(onClose, void());
};

}
}

#endif //eMUUT_EVENTHANDLERMOCK_HPP
#include "ioServiceMock.hpp"

namespace eMUNetworkUT = eMUUnitTest::networkTest;

void eMUNetworkUT::ioServiceMock_t::delegateMocks() {
    ON_CALL(*this, read(::testing::_,
                        ::testing::_,
                        ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                       &ioServiceMock_t::readImpl));
    ON_CALL(*this, write(::testing::_,
                         ::testing::_,
                         ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                        &ioServiceMock_t::writeImpl));

    ON_CALL(*this, accept(::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                         &ioServiceMock_t::acceptImpl));
}

void eMUNetworkUT::ioServiceMock_t::dequeueRead(const boost::system::error_code& ec,
                                                size_t bytesTransferred) {
    readHandler_(ec, bytesTransferred);
}

void eMUNetworkUT::ioServiceMock_t::dequeueWrite(const boost::system::error_code& ec,
                                                 size_t bytesTransferred) {
    writeHandler_(ec, bytesTransferred);
}

void eMUNetworkUT::ioServiceMock_t::dequeueAccept(const boost::system::error_code &ec) {
    acceptHandler_(ec);
}

void eMUNetworkUT::ioServiceMock_t::readImpl(uint8 *payload, size_t size, ioHandler_t handler) {
    readHandler_ = handler;
}

void eMUNetworkUT::ioServiceMock_t::writeImpl(const uint8 *payload, size_t size, ioHandler_t handler) {
    writeHandler_ = handler;
}

void eMUNetworkUT::ioServiceMock_t::acceptImpl(acceptHandler_t handler) {
    acceptHandler_ = handler;      
}
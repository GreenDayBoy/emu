#include "ioServiceMock.hpp"

void eMUUnitTest::networkTest::ioServiceMock_t::delegateMocks() {
    ON_CALL(*this, read(::testing::_,
                        ::testing::_,
                        ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                       &ioServiceMock_t::readImpl));
    ON_CALL(*this, write(::testing::_,
                         ::testing::_,
                         ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                        &ioServiceMock_t::writeImpl));
}

void eMUUnitTest::networkTest::ioServiceMock_t::dequeueRead(const boost::system::error_code& ec,
                                                            size_t bytesTransferred) {
    readHandler_(ec, bytesTransferred);
}

void eMUUnitTest::networkTest::ioServiceMock_t::dequeueWrite(const boost::system::error_code& ec,
                                                             size_t bytesTransferred) {
    writeHandler_(ec, bytesTransferred);
}

void eMUUnitTest::networkTest::ioServiceMock_t::readImpl(uint8 *payload, size_t size, ioHandler_t handler) {
    readHandler_ = handler;
}

void eMUUnitTest::networkTest::ioServiceMock_t::writeImpl(const uint8 *payload, size_t size, ioHandler_t handler) {
    writeHandler_ = handler;
}
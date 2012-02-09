#ifndef eMUUT_IOSERVICEMOCK_HPP
#define eMUUT_IOSERVICEMOCK_HPP

#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include "../shared/types.hpp"

namespace eMUUnitTest {
namespace networkTest {

class ioServiceMock_t {
public:
    typedef boost::function2<void, const boost::system::error_code&, size_t> ioHandler_t;

    void delegateMocks();
    void dequeueRead(const boost::system::error_code& ec,
                     size_t bytesTransferred);
    void dequeueWrite(const boost::system::error_code& ec,
                      size_t bytesTransferred);

    MOCK_METHOD1(shutdownSocket, void(boost::asio::ip::tcp::socket::shutdown_type type));
    MOCK_METHOD3(read, void(uint8 *payload, size_t size, ioHandler_t handler));
    MOCK_METHOD3(write, void(const uint8 *payload, size_t size, ioHandler_t handler));

private:
    void readImpl(uint8 *payload, size_t size, ioHandler_t handler);
    void writeImpl(const uint8 *payload, size_t size, ioHandler_t handler);

    ioHandler_t readHandler_;
    ioHandler_t writeHandler_;
};

}
}

#endif //eMUUT_IOSERVICEMOCK_HPP
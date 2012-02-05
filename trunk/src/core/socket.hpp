#ifndef eMUCORE_SOCKET_HPP
#define eMUCORE_SOCKET_HPP

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include "core.hpp"
#include "../shared/types.hpp"

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

namespace eMUCore {
namespace network {

class eMUCORE_DECLSPEC socket_t: private boost::noncopyable {
public:
    typedef boost::function2<void, const boost::system::error_code&, size_t> ioHandler_t;

    socket_t(boost::asio::ip::tcp::socket *socket);
    virtual ~socket_t() {}

    MOCKABLE void close();
    MOCKABLE void queueRead(uint8 *payload, size_t size, ioHandler_t handler);
    MOCKABLE void queueWrite(const uint8 *payload, size_t size, ioHandler_t handler);

private:
    socket_t();

    boost::asio::ip::tcp::socket *socket_;
};

};
};

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif // eMUCORE_SOCKET_HPP
#ifndef eMUCORE_CONNECTION_HPP
#define eMUCORE_CONNECTION_HPP

#include <boost/noncopyable.hpp>
#include "buffer.hpp"
#include "socket.hpp"

#ifdef WIN32
#pragma warning(disable: 4275)
#endif

namespace eMUCore {
namespace network {

class eMUCORE_DECLSPEC connection_t: private boost::noncopyable {
public:
    class eventHandler_t {
    public:
        virtual ~eventHandler_t() {}
        virtual void onReceive(uint8 *payload, size_t size) = 0;
        virtual void onClose() = 0;
    };
    
    connection_t(socket_t *socket,
                 eventHandler_t *eventHandler);
    virtual ~connection_t() {}

    void send(const uint8 *payload, size_t size);
    void disconnect();

private:
    connection_t();

    void receiveHandler(const boost::system::error_code& ec,
                        size_t bytesTransferred);
    void sendHandler(const boost::system::error_code& ec,
                     size_t bytesTransferred);

    socket_t *socket_;
    eventHandler_t *eventHandler_;
    readBuffer_t rbuf_;
    writeBuffer_t wbuf_;
};

};
};

#ifdef WIN32
#pragma warning(default: 4275)
#endif

#endif // eMUCORE_CONNECTION_HPP
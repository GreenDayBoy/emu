#ifndef eMUCORE_PEER_HPP
#define eMUCORE_PEER_HPP

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "socket.hpp"

#ifdef WIN32
#pragma warning(disable: 4355)
#endif

namespace eMUCore {
namespace network {

template<typename SocketImpl = socket_t<> >
class peer_t
    : private boost::noncopyable,
      public boost::enable_shared_from_this<peer_t<SocketImpl> > {
public:
    typedef boost::shared_ptr<peer_t<SocketImpl> > ptr_t;

    peer_t(uint32 id):
      id_(id) {}

    virtual ~peer_t() {}

    virtual void send(const uint8 *payload, size_t size) {
        socket_->send(payload, size);
    }

    virtual void disconnect() {
        if(socket_->opened()) {
            socket_->close();
            socket_.reset();
        }
    }

    void socket(typename SocketImpl::ptr_t socket) {
        socket_ = socket;
        socket_->queueRead();
    }

    void address() {
        socket_->address();
    }

    bool connected() {
        if(!socket_)
            return false;
        else
            return socket_->opened();
    }

    bool operator==(typename const SocketImpl::ptr_t socket) {
        return socket_ == socket;
    }

    uint32 id() { return id_; }

protected:
    peer_t();

    typename SocketImpl::ptr_t socket_;
    uint32 id_;
};

}
}

#ifdef WIN32
#pragma warning(default: 4355)
#endif

#endif
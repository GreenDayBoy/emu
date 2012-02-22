#ifndef eMUCORE_SERVERENTITY_HPP
#define eMUCORE_SERVERENTITY_HPP

#include <boost/noncopyable.hpp>
#include "../shared/types.hpp"
#include "socket.hpp"

namespace eMUCore {
namespace network {

template<typename SocketImpl = socket_t<> >
class serverEntity_t: private boost::noncopyable {
public:
    virtual ~serverEntity_t() {}
    virtual void onPeerConnect(typename SocketImpl::ptr_t socket) = 0;
    virtual void onPeerReceive(typename SocketImpl::ptr_t socket) = 0;
    virtual void onPeerClose(typename SocketImpl::ptr_t socket) = 0;
};

};
};

#endif
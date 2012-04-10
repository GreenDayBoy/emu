#ifndef eMU_CORE_CONNECTIONOBSERVER_HPP
#define eMU_CORE_CONNECTIONOBSERVER_HPP

#include "connection.hpp"

namespace eMU {
namespace core {
namespace network {

class connectionObserver_i {
public:
    virtual ~connectionObserver_i() {}
    virtual void onConnect(ptr_t connection) = 0;
    virtual void onReceive(ptr_t connection, payload_t &payload) = 0;
    virtual void onClose(ptr_t connection) = 0;
};

}
}
}

#endif
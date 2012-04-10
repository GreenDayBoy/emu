#ifndef eMU_CORE_USER_HPP
#define eMU_CORE_USER_HPP

#include "connection.hpp"

namespace eMU {
namespace core {
namespace user {

template<typename ConnectionImpl = network::connection_t<>>
class user_t: private boost::noncopyable {
public:
    user_t(int16 id):
      id_(id),
      connection_(NULL) {}
    virtual ~user_t() {}

    int16 id() { return id_; }

    ConnectionImpl* connection() { return connection_; }
    void connection(ConnectionImpl *connection) { connection_ = connection; }

    bool operator==(ConnectionImpl *connection) const {
        return connection_ == connection;
    }

protected:
    user_t();

    ConnectionImpl *connection_;
    int16 id_;
};

}
}
}

#endif
#ifndef eMU_CORE_USER_HPP
#define eMU_CORE_USER_HPP

#include "tcpConnection.hpp"

namespace eMU {
namespace core {
namespace user {

template<typename ConnectionImpl = network::tcp::connection_t<> >
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

    friend std::ostream& operator<<(std::ostream &out, const user_t &user) {
        out << "user id: " << user.id_ << ", address: " << user.connection_->address();
        return out;
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
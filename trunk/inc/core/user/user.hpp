#pragma once

#include <network/tcp/tcpConnection.hpp>

namespace eMU {
namespace core {
namespace user {

class user_t: private boost::noncopyable {
public:
    user_t(int16 id):
      id_(id),
      connection_(NULL) {}

    virtual ~user_t() {}

    int16 id() { return id_; }
    network::tcp::connection_t<>* connection() { return connection_; }
    void connection(network::tcp::connection_t<> *connection) { connection_ = connection; }

    friend std::ostream& operator<<(std::ostream &out, const user_t &user) {
        out << "id: " << user.id_ << ", address: " << user.connection_->address();
        return out;
    }

    bool operator==(const network::tcp::connection_t<> *connection) { return connection_ == connection; }

protected:
    user_t();

    network::tcp::connection_t<> *connection_;
    int16 id_;
};

}
}
}

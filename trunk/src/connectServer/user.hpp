#ifndef eMU_CONNECTSERVER_USER_HPP
#define eMU_CONNECTSERVER_USER_HPP

#include <boost/noncopyable.hpp>
#include "../shared/types.hpp"
#include "../core/connection.hpp"

namespace eMU {
namespace connectServer {

class user_t: private boost::noncopyable {
public:
    user_t(int16 id);

    eMU::core::network::connection_t<>* connection();

private:
    int16 id_;
    eMU::core::network::connection_t<> *connection_;
};

}
}
#endif
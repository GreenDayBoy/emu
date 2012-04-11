#ifndef eMU_CONNECTSERVER_USER_HPP
#define eMU_CONNECTSERVER_USER_HPP

#include <boost/noncopyable.hpp>
#include "../shared/types.hpp"
#include "../core/user.hpp"

namespace eMU {
namespace connectServer {

class user_t: public eMU::core::user::user_t<> {
public:
    user_t(int16 id);

private:
};

}
}
#endif
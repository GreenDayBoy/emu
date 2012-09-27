#pragma once

#include <boost/noncopyable.hpp>

#include <shared/types.hpp>
#include <core/user/user.hpp>

namespace eMU {
namespace connectServer {

class user_t: public eMU::core::user::user_t {
public:
    user_t(int16 id);

private:
};

}
}

#pragma once

#include <shared/types.hpp>
#include <core/user/user.hpp>

namespace eMU {
namespace ut {
namespace env {
namespace core {
namespace user {

class userStub_t: public eMU::core::user::user_t {
public:
    userStub_t(int16 id):
      user_t(id) {}

private:
    userStub_t();
};

}
}
}
}
}

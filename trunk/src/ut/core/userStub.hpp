#ifndef eMU_UT_USERSTUB_HPP
#define eMU_UT_USERSTUB_HPP

#include "socketMock.hpp"
#include "ioServiceStub.hpp"
#include "../../core/connection.hpp"
#include "../../core/user.hpp"

namespace eMU {
namespace ut {
namespace network {

class userStub_t: public eMU::core::user::user_t<eMU::core::network::connection_t<socketMock_t, ioServiceStub_t> > {
public:
    userStub_t(int16 id):
      user_t(id) {}

private:
    userStub_t();
};

}
}
}

#endif
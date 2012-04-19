#ifndef eMU_UT_USERSTUB_HPP
#define eMU_UT_USERSTUB_HPP

#include "tcpSocketMock.hpp"
#include "ioServiceStub.hpp"
#include "types.hpp"
#include "../../core/tcpConnection.hpp"
#include "../../core/user.hpp"

namespace eMU {
namespace ut {
namespace network {

class userStub_t: public eMU::core::user::user_t<tcp::testConnection_t> {
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
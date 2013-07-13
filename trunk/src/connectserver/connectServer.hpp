#pragma once

#include <core/network/tcp/connectionsManager.hpp>
#include <core/network/udp/connection.hpp>
#include <core/common/usersFactory.hpp>
#include <user.hpp>

#ifdef eMU_UT
#include <ut/env/asioStub/ioService.hpp>
#elif eMU_MT
#include <mt/env/asioStub/ioService.hpp>
#else
#include <boost/asio.hpp>
#endif

namespace eMU {
namespace connectserver {

#ifdef eMU_UT
namespace asio = eMU::ut::env::asioStub;
#elif eMU_MT
namespace asio = eMU::mt::env::asioStub;
#else
namespace asio = boost::asio;
#endif

class Server: boost::noncopyable {
public:
    Server(asio::io_service &ioService, int16_t port, size_t maxNumberOfUsers);
    void startup();

    size_t generateConnectionHash();
    void onAccept(size_t hash);
    void onReceive(size_t hash);
    void onClose(size_t hash);
    void onReceiveFrom(core::network::udp::Connection &connection);

private:
    core::network::tcp::ConnectionsManager connectionsManager_;
    core::network::udp::Connection udpConnection_;
    core::common::UsersFactory<User> usersFactory_;

};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

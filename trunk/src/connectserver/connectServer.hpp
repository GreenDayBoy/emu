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
    Server(core::network::tcp::ConnectionsManager::Pointer connectionsManager,
           core::common::UsersFactory<User>::Pointer usersFactory,
           core::network::udp::Connection::Pointer udpConnection);

    void startup();
    void cleanup();

    size_t generateConnectionHash();
    void onAccept(size_t hash);
    void onReceive(size_t hash, const eMU::core::network::Payload &payload);
    void onClose(size_t hash);
    void onReceiveFrom(core::network::udp::Connection &connection);

private:
    core::network::tcp::ConnectionsManager::Pointer connectionsManager_;
    core::common::UsersFactory<User>::Pointer usersFactory_;
    core::network::udp::Connection::Pointer udpConnection_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif
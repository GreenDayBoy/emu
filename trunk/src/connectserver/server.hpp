#pragma once

#ifdef eMU_UT
#include <ut/env/asioStub/ioService.hpp>
#elif eMU_MT
#include <mt/env/asioStub/ioService.hpp>
#else
#include <boost/asio.hpp>
#endif

#include <core/network/tcp/connectionsManager.hpp>
#include <core/network/udp/connection.hpp>
#include <core/network/server.hpp>
#include <core/common/usersFactory.hpp>
#include <core/transactions/manager.hpp>
#include <connectserver/user.hpp>
#include <connectserver/gameServersList.hpp>
#include <connectserver/messageSender.hpp>

namespace eMU
{
namespace connectserver
{

#ifdef eMU_UT
namespace asio = eMU::ut::env::asioStub;
#elif eMU_MT
namespace asio = eMU::mt::env::asioStub;
#else
namespace asio = boost::asio;
#endif

class Server: public core::network::Server<User>
{
public:
    struct Configuration
    {
        uint16_t port_;
        size_t maxNumberOfUsers_;
        std::string gameServersListContent_;
    };

    Server(asio::io_service &ioService, const Configuration &configuration);
    Server(core::network::tcp::ConnectionsManager::Pointer connectionsManager,
           core::common::UsersFactory<User>::Pointer usersFactory,
           core::transactions::Manager::Pointer transactionsManager,
           core::network::udp::Connection::Pointer udpConnection,
           const Configuration &configuration);

    void startup();
    void cleanup();
    void onAccept(size_t hash);
    void onClose(size_t hash);

private:
    void handleMessage(size_t hash, const eMU::core::network::Payload &payload);
    void onReceiveFrom(core::network::udp::Connection &connection);

    core::network::udp::Connection::Pointer udpConnection_;
    GameServersList gameServersList_;
    MessageSender messageSender_;
    std::string gameServersListContent_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

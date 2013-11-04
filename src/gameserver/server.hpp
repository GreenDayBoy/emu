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
#include <gameserver/user.hpp>
#include <gameserver/messageSender.hpp>

namespace eMU
{
namespace gameserver
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
        std::string connectServerAddress_;
        uint16_t connectServerPort_;
        size_t loadReportDuration_;
    };

    Server(asio::io_service &ioService, const Configuration &configuration);

    bool startup();
    void cleanup();
    void onAccept(size_t hash);
    void onClose(size_t hash);
    void onReceiveFrom(core::network::udp::Connection &connection);

    void onDataServerConnect(core::network::tcp::Connection &connection);
    void onDataServerReceive(core::network::tcp::Connection &connection);
    void onDataServerClose(core::network::tcp::Connection &connection);

private:
    void handleMessage(size_t hash, const eMU::core::network::Payload &payload);

    Configuration configuration_;
    core::network::tcp::Connection::Pointer dataServerConnection_;
    core::network::udp::Connection::Pointer udpConnection_;
    MessageSender messageSender_;

};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

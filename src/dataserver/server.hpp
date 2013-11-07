#pragma once

#include <core/network/tcp/connectionsManager.hpp>
#include <core/network/server.hpp>
#include <common/asio.hpp>
#include <dataserver/user.hpp>

namespace eMU
{
namespace dataserver
{

class Server: public core::network::Server<User>
{
public:
    struct Configuration
    {
        uint16_t port_;
        size_t maxNumberOfUsers_;
    };

    Server(asio::io_service &ioService, const Configuration &configuration);

    bool onStartup();
    void onCleanup();
    void onAccept(size_t hash);
    void onClose(size_t hash);

private:
    void handlePacket(size_t hash, const eMU::core::network::Payload &packet);
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

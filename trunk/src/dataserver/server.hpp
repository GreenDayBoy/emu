#pragma once

#include <core/network/tcp/connectionsManager.hpp>
#include <core/network/server.hpp>
#include <common/asio.hpp>
#include <dataserver/user.hpp>
#include <dataserver/database/sqlInterface.hpp>
#include <dataserver/messageSender.hpp>

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

    Server(asio::io_service &ioService, database::SqlInterface &sqlInterface, const Configuration &configuration);

    bool onStartup();
    void onCleanup();
    void onAccept(size_t hash);
    void onClose(size_t hash);

private:
    void handlePacket(size_t hash, core::network::Payload &packet);

    database::SqlInterface &sqlInterface_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

#pragma once

#include <core/network/server.hpp>
#include <core/network/udp/connection.hpp>
#include <protocol/readStream.hpp>
#include <common/asio.hpp>
#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
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
    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);
    void onLoginserverReceive(core::network::udp::Connection &connection);

private:
    void handleReadStream(User &user, const protocol::ReadStream &stream);

    core::network::udp::Connection loginserverConnection_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

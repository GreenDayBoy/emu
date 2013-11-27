#pragma once

#include <core/network/tcp/connectionsManager.hpp>
#include <core/network/server.hpp>
#include <protocol/readStream.hpp>
#include <common/asio.hpp>
#include <loginserver/user.hpp>

namespace eMU
{
namespace loginserver
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
    void onReceive(size_t hash, const core::network::Payload &payload);
    void onClose(size_t hash);

private:
    void handleReadStream(size_t hash, const protocol::ReadStream &stream);

    void onDataserverConnect(core::network::tcp::Connection &connection);
    void onDataserverReceive(core::network::tcp::Connection &connection);
    void onDataserverClose(core::network::tcp::Connection &connection);
    void handleDataserverReadStream(const protocol::ReadStream &stream);

    core::network::tcp::Connection dataserverConnection_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

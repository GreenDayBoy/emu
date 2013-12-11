#pragma once

#include <core/network/server.hpp>
#include <protocol/readStream.hpp>
#include <common/asio.hpp>
#include <dataserver/user.hpp>
#include <dataserver/database/sqlInterface.hpp>

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
    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);

private:
    void handleReadStream(User &user, const protocol::ReadStream &stream);

    database::SqlInterface &sqlInterface_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

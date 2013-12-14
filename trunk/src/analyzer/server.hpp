#pragma once

#include <analyzer/user.hpp>
#include <protocol/readStream.hpp>
#include <core/network/server.hpp>

namespace eMU
{
namespace analyzer
{

class Controller;

class Server: public core::network::Server<User>
{
public:
    Server(asio::io_service &ioService);

    bool onStartup();
    void onCleanup();
    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);

    void setController(Controller *controller);

private:
    Controller *controller_;
};

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]);
#endif

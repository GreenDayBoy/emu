#pragma once

#include <core/network/udp/udpConnection.hpp>
#include <core/network/server.hpp>
#include <user.hpp>

namespace eMU {
namespace connectServer {

class server_t: public eMU::core::network::server_t<user_t> {
public:
    server_t(boost::asio::io_service &ioService, uint16 port, size_t maxNumOfUsers);

    void onStartup();
    void onCleanup();

    bool onAccept(user_t &user);
    void onReceive(user_t &user);
    void onClose(user_t &user);

    void onReceiveFrom(eMU::core::network::udp::connection_t<> *connection,
                       const boost::asio::ip::udp::endpoint &endpoint);

private:
    eMU::core::network::udp::connection_t<> udpConnection_;
};

}
}

int main(int argsCount, char *args[]);

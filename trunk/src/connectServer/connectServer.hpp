#ifndef eMU_CONNECTSERVER_CONNECTSERVER_HPP
#define eMU_CONNECTSERVER_CONNECTSERVER_HPP

#include "../core/server.hpp"
#include "user.hpp"

namespace eMU {
namespace connectServer {

class server_t: public eMU::core::network::server_t<user_t> {
public:
    server_t(boost::asio::io_service &ioService, uint16 port, size_t maxNumOfUsers);

    void onStartup();
    void onCleanup();

    bool onConnect(user_t *user);
    void onReceive(user_t *user, eMU::core::network::payload_t &payload);
    void onClose(user_t *user);
};

}
}

void main(int argsCount, char *args[]);

#endif 
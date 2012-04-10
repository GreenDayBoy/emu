#ifndef eMU_CONNECTSERVER_CONNECTSERVER_HPP
#define eMU_CONNECTSERVER_CONNECTSERVER_HPP

#include "../core/server.hpp"

namespace eMU {
namespace connectServer {

class server_t: public eMU::core::network::server_t<> {
public:
    server_t(boost::asio::io_service &ioService, uint16 port);

    void onStartup();
    void onCleanup();

    void onConnect(eMU::core::network::connection_t<> *connection);
    void receiveEvent(eMU::core::network::connection_t<> *connection, eMU::core::network::payload_t &payload);
    void closeEvent(eMU::core::network::connection_t<> *connection);
};

}
}

void main(int argsCount, char *args[]);

#endif 
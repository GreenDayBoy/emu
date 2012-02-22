#include "server.hpp"

class myEntity: public eMUCore::network::serverEntity_t<> {
public:
    void onPeerConnect(eMUCore::network::socket_t<>::ptr_t socket) {

    }

    void onPeerReceive(eMUCore::network::socket_t<>::ptr_t socket) {

    }

    void onPeerClose(eMUCore::network::socket_t<>::ptr_t socket) {

    }
};

void asd() {
    boost::asio::io_service service;
    myEntity entity;
    eMUCore::network::server_t<> server(service, entity);
    server.initialize();
}
#include <boost/lexical_cast.hpp>
#include "../core/serviceThreading.hpp"
#include "../core/baseApplication.hpp"
#include "connectServer.hpp"

eMU::connectServer::server_t::server_t(boost::asio::io_service &ioService, uint16 port):
  eMU::core::network::server_t<>(ioService, port) {

}

void eMU::connectServer::server_t::onStartup() {
    this->queueAccept();
}

void eMU::connectServer::server_t::onCleanup() {
}

void eMU::connectServer::server_t::onConnect(eMU::core::network::connection_t<> *connection) {
}

void eMU::connectServer::server_t::receiveEvent(eMU::core::network::connection_t<> *connection, eMU::core::network::payload_t &payload) {
}

void eMU::connectServer::server_t::closeEvent(eMU::core::network::connection_t<> *connection) {
}

void main(int argsCount, char *args[]) {
    if(argsCount != 4) {
        LOG_ERROR << "Invalid command line for start ConnectServer instance." << std::endl;
        return;
    }

    size_t maxNumOfUsers = boost::lexical_cast<size_t>(args[1]);
    uint16 port = boost::lexical_cast<uint16>(args[2]);
    size_t maxNumOfThreads = boost::lexical_cast<uint16>(args[3]);

    boost::asio::io_service ioService;
    eMU::connectServer::server_t server(ioService, port);

    eMU::core::baseApplication_t app(ioService, server);
    app.initialize(maxNumOfThreads, maxNumOfUsers);
    app.start();

    system("PAUSE");
}
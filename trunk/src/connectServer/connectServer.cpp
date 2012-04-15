#include <boost/lexical_cast.hpp>
#include "../core/baseApplication.hpp"
#include "connectServer.hpp"
#include "user.hpp"

eMU::connectServer::server_t::server_t(boost::asio::io_service &ioService, uint16 port, size_t maxNumOfUsers):
  eMU::core::network::server_t<user_t>(ioService, port, maxNumOfUsers) {}

void eMU::connectServer::server_t::onStartup() {
    LOG_INFO << "Connect server started." << std::endl;
}

void eMU::connectServer::server_t::onCleanup() {
}

bool eMU::connectServer::server_t::onAccept(user_t *user) {
    LOG_INFO << "User connected: " << *user << std::endl;
    return true;
}

void eMU::connectServer::server_t::onReceive(user_t *user, eMU::core::network::payload_t &payload) {
    LOG_INFO << "User received: " << *user << std::endl;
}

void eMU::connectServer::server_t::onClose(user_t *user) {
    LOG_INFO << "User closed from: " << user->connection()->address() << ", id: " << user->id() << std::endl;
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

    eMU::connectServer::server_t server(ioService, port, maxNumOfUsers);
    server.queueAccept();

    eMU::core::baseApplication_t<eMU::connectServer::user_t> app(ioService, server, maxNumOfThreads);
    app.start();

    system("PAUSE");
}
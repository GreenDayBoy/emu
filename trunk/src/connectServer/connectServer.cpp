#include <boost/lexical_cast.hpp>

#include <core/baseApplication.hpp>
#include <connectServer.hpp>

eMU::connectServer::server_t::server_t(boost::asio::io_service &ioService, uint16 port, size_t maxNumOfUsers):
  eMU::core::network::server_t<user_t>(ioService, port, maxNumOfUsers),
  udpConnection_(ioService, port) {
    udpConnection_.receiveFromEventCallback(boost::bind(&server_t::onReceiveFrom, this, _1, _2));
}

void eMU::connectServer::server_t::onStartup() {
    LOG_INFO << "Connect server started." << std::endl;
}

void eMU::connectServer::server_t::onCleanup() {
    LOG_INFO << "Connect server goes down." << std::endl;
}

bool eMU::connectServer::server_t::onAccept(user_t &user) {
    LOG_INFO << "Connected, user: " << user << std::endl;
    return true;
}

void eMU::connectServer::server_t::onReceive(user_t &user) {
    LOG_INFO << "Received, user: " << user << std::endl;
}

void eMU::connectServer::server_t::onClose(user_t &user) {
    LOG_INFO << "Closed, user: " << user << std::endl;
}

void eMU::connectServer::server_t::onReceiveFrom(eMU::core::network::udp::connection_t<> *connection,
                                                 const boost::asio::ip::udp::endpoint &endpoint) {
}

int main(int argsCount, char *args[]) {
    if(argsCount != 4) {
        LOG_ERROR << "Invalid command line for start ConnectServer instance." << std::endl;
        return 0;
    }

    size_t maxNumOfUsers = boost::lexical_cast<size_t>(args[1]);
    uint16 port = boost::lexical_cast<uint16>(args[2]);
    size_t maxNumOfThreads = boost::lexical_cast<uint16>(args[3]);

    eMU::core::baseApplication_t<eMU::connectServer::server_t> app(maxNumOfThreads, port, maxNumOfUsers);
    app.start();

#ifdef WIN32
    system("PAUSE");
#endif

    return 0;
}
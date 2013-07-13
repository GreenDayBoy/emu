#include <connectserver/connectServer.hpp>
#include <core/common/serviceThreading.hpp>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU {
namespace connectserver {

Server::Server(asio::io_service &ioService, int16_t port, size_t maxNumberOfUsers):
  connectionsManager_(ioService, port),
  udpConnection_(ioService, port),
  usersFactory_(maxNumberOfUsers) {
    connectionsManager_.setAcceptEventCallback(std::bind(&Server::onAccept, this, std::placeholders::_1));
    connectionsManager_.setReceiveEventCallback(std::bind(&Server::onReceive, this, std::placeholders::_1));
    connectionsManager_.setCloseEventCallback(std::bind(&Server::onClose, this, std::placeholders::_1));
    connectionsManager_.setGenerateConnectionHashCallback(std::bind(&Server::generateConnectionHash, this));

    udpConnection_.setReceiveFromEventCallback(std::bind(&Server::onReceiveFrom, this, std::placeholders::_1));
}

void Server::startup() {

}

size_t Server::generateConnectionHash() {
    User &user = usersFactory_.create();

    return user.hash();
}

void Server::onAccept(size_t hash) {
    User &user = usersFactory_.find(hash);
}

void Server::onReceive(size_t hash) {
    User &user = usersFactory_.find(hash);
}

void Server::onClose(size_t hash) {
    User &user = usersFactory_.find(hash);
}

void Server::onReceiveFrom(core::network::udp::Connection &connection) {

}

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[]) {
    if(argsCount != 4) {
        LOG(ERROR) << "Invalid command line to start ConnectServer instance.";
        return 1;
    }

    google::InitGoogleLogging(args[0]);

    size_t maxNumberOfUsers = boost::lexical_cast<size_t>(args[1]);
    uint16_t port = boost::lexical_cast<uint16_t>(args[2]);

    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);
    eMU::core::common::ServiceThreading serviceThreading(maxNumberOfThreads);

    return 0;
}
#endif

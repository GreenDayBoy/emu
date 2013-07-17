#include <connectserver/connectServer.hpp>
#include <core/application/starter.hpp>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU {
namespace connectserver {

Server::Server(asio::io_service &ioService, int16_t port, size_t maxNumberOfUsers):
  connectionsManager_(ioService, port),
  udpConnection_(ioService, port),
  usersFactory_(maxNumberOfUsers) {
    connectionsManager_.setAcceptEventCallback(std::bind(&Server::onAccept, this, std::placeholders::_1));
    connectionsManager_.setReceiveEventCallback(std::bind(&Server::onReceive, this, std::placeholders::_1, std::placeholders::_2));
    connectionsManager_.setCloseEventCallback(std::bind(&Server::onClose, this, std::placeholders::_1));
    connectionsManager_.setGenerateConnectionHashCallback(std::bind(&Server::generateConnectionHash, this));

    udpConnection_.setReceiveFromEventCallback(std::bind(&Server::onReceiveFrom, this, std::placeholders::_1));
}

void Server::startup() {
    connectionsManager_.queueAccept();
}

size_t Server::generateConnectionHash() {
    try {
        User &user = usersFactory_.create();

        return user.hash();
    } catch(eMU::core::common::Exception &exception) {
        throw exception;
    }
}

void Server::onAccept(size_t hash) {
    User &user = usersFactory_.find(hash);

    LOG(INFO) << "User registered, hash: " << hash;
}

void Server::onReceive(size_t hash, const eMU::core::network::Payload &payload) {
    User &user = usersFactory_.find(hash);

    LOG(INFO) << "User received, hash: " << hash << ", size: " << payload.size();
}

void Server::onClose(size_t hash) {
    User &user = usersFactory_.find(hash);

    LOG(INFO) << "User closed, hash: " << hash;

    usersFactory_.destroy(hash);
}

void Server::onReceiveFrom(core::network::udp::Connection &connection) {

}

void Server::cleanup() {

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
    eMU::core::application::Starter<eMU::connectserver::Server> starter(maxNumberOfThreads, port, maxNumberOfUsers);
    starter.start();

    return 0;
}
#endif

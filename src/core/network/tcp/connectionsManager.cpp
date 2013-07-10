#include <glog/logging.h>
#include <network/tcp/connectionsManager.hpp>
#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace network {
namespace tcp {

ConnectionsManager::ConnectionsManager(asio::io_service &ioService, int16_t port):
  ioService_(ioService),
  acceptor_(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {

}

ConnectionsManager::~ConnectionsManager() {

}

void ConnectionsManager::queueAccept() {
    Connection::Pointer connection(new Connection(ioService_));

    acceptor_.async_accept(connection->socket(),
                           std::bind(&ConnectionsManager::acceptHandler,
                                     this,
                                     connection,
                                     std::placeholders::_1));
}

void ConnectionsManager::setGenerateConnectionHashCallback(const GenerateConnectionHashCallback &callback) {
    generateConnectionHashCallback_ = callback;
}

void ConnectionsManager::acceptHandler(Connection::Pointer connection, const boost::system::error_code &errorCode) {
    if(errorCode) {
        LOG(ERROR) << "Error during establishing connection, error: " << errorCode.message();
    } else {
        this->registerConnection(connection);
    }

    this->queueAccept();
}

void ConnectionsManager::registerConnection(Connection::Pointer connection) {
    try {
        size_t hash = generateConnectionHashCallback_();
        connections_[hash] = connection;

        connection->setReceiveEventCallback(std::bind(&ConnectionsManager::receiveEvent, this, std::placeholders::_1));
        connection->setCloseEventCallback(std::bind(&ConnectionsManager::closeEvent, this, std::placeholders::_1));
        connection->queueReceive();

        acceptEventCallback_(hash);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during regustering connection, reason: " << exception.what();
    }
}

void ConnectionsManager::setAcceptEventCallback(const AcceptEventCallback &callback) {
    acceptEventCallback_ = callback;
}

void ConnectionsManager::setReceiveEventCallback(const ReceiveEventCallback &callback) {
    receiveEventCallback_ = callback;
}

void ConnectionsManager::setCloseEventCallback(const CloseEventCallback &callback) {
    closeEventCallback_ = callback;
}

void ConnectionsManager::send(size_t hash, const Payload &payload)
{
    connections_[hash]->send(payload);
}

void ConnectionsManager::receiveEvent(Connection &connection) {
    try {
        size_t hash = this->findConnectionHash(connection);

        Payload payload(connection.readBuffer().payload_.begin(), connection.readBuffer().payload_.begin() + connection.readBuffer().payloadSize_);

        receiveEventCallback_(hash, payload);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during receiveEvent, reason: " << exception.what();
    }
}

void ConnectionsManager::closeEvent(Connection &connection) {
    try {
        size_t hash = this->findConnectionHash(connection);
        closeEventCallback_(hash);
        connections_.erase(hash);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during closeEvent, reason: " << exception.what();
    }
}

size_t ConnectionsManager::findConnectionHash(const Connection &connection) const {
    std::map<size_t, Connection::Pointer>::const_iterator it = connections_.begin();

    for(; it != connections_.end(); it++) {
        if(it->second->hash() == connection.hash()) {
            return it->first;
        }
    }

    common::Exception exception;
    exception.in() << "Could not find connection, hash: " << connection.hash() << ", address: " << connection.address();
    throw exception;
}

#ifdef eMU_UT
asio::ip::tcp::acceptor& ConnectionsManager::acceptor() {
    return acceptor_;
}
#endif

void ConnectionsManager::disconnect(size_t hash) {
    connections_[hash]->disconnect();
}

}
}
}
}

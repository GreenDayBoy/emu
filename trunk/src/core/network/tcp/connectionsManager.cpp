#include <glog/logging.h>
#include <network/tcp/connectionsManager.hpp>
#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace network {
namespace tcp {

ConnectionsManager::ConnectionsManager(asio::io_service &ioService, int16_t port):
  ConnectionsManager(ConnectionsFactory::Pointer(new ConnectionsFactory),
                     ioService,
                     AcceptorPointer(new asio::ip::tcp::acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)))) {}

ConnectionsManager::ConnectionsManager(ConnectionsFactory::Pointer connectionsFactory, asio::io_service &ioService, AcceptorPointer acceptor):
  connectionsFactory_(connectionsFactory),
  ioService_(ioService),
  acceptor_(acceptor) {}

void ConnectionsManager::queueAccept() {
    Connection::SocketPointer socket(new asio::ip::tcp::socket(ioService_));

    acceptor_->async_accept(*socket,
                            std::bind(&ConnectionsManager::acceptHandler,
                                      this,
                                      socket,
                                      std::placeholders::_1));
}

void ConnectionsManager::setGenerateConnectionHashCallback(const GenerateConnectionHashCallback &callback) {
    generateConnectionHashCallback_ = callback;
}

void ConnectionsManager::acceptHandler(Connection::SocketPointer socket, const boost::system::error_code &errorCode) {
    if(errorCode) {
        LOG(ERROR) << "Error during establishing connection, error: " << errorCode.message();
    } else {
        this->registerConnection(socket);
    }

    this->queueAccept();
}

void ConnectionsManager::registerConnection(Connection::SocketPointer socket) {
    try {
        size_t hash = generateConnectionHashCallback_();
        Connection &connection = connectionsFactory_->create(hash, socket);

        LOG(INFO) << "Connection registered, hash: " << hash;

        connection.setReceiveEventCallback(std::bind(&ConnectionsManager::receiveEvent, this, std::placeholders::_1));
        connection.setCloseEventCallback(std::bind(&ConnectionsManager::closeEvent, this, std::placeholders::_1));
        connection.queueReceive();

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

void ConnectionsManager::send(size_t hash, const Payload &payload) {
    try {
        Connection &connection = connectionsFactory_->get(hash);
        connection.send(payload);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during send, reason: " << exception.what();
    }
}

void ConnectionsManager::receiveEvent(Connection &connection) {
    try {
        size_t hash = connectionsFactory_->getHash(connection);
        Payload payload(connection.readBuffer().payload_.begin(), connection.readBuffer().payload_.begin() + connection.readBuffer().payloadSize_);

        receiveEventCallback_(hash, payload);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during receiveEvent, reason: " << exception.what();
    }
}

void ConnectionsManager::closeEvent(Connection &connection) {
    try {
        LOG(INFO) << "Closing connection: " << connection;

        size_t hash = connectionsFactory_->getHash(connection);

        closeEventCallback_(hash);
        connection.close();
        connectionsFactory_->destroy(hash);
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during closeEvent, reason: " << exception.what();
    }
}

void ConnectionsManager::disconnect(size_t hash) {
    try {
        Connection &connection = connectionsFactory_->get(hash);
        connection.disconnect();
    } catch(common::Exception &exception) {
        LOG(ERROR) << "Exception during disconnect, reason: " << exception.what();
    }
}

}
}
}
}

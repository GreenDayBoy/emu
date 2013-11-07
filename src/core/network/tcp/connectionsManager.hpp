#pragma once

#include <core/network/writeBuffer.hpp>
#include <core/network/readBuffer.hpp>
#include <core/network/tcp/connectionsFactory.hpp>
#include <common/asio.hpp>

#include <boost/noncopyable.hpp>
#include <functional>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class ConnectionsManager: boost::noncopyable
{
public:
    typedef std::function<size_t()> GenerateConnectionHashCallback;
    typedef std::function<void(size_t, const Payload&)> ReceiveEventCallback;
    typedef std::function<void(size_t)> CloseEventCallback;
    typedef std::function<void(size_t)> AcceptEventCallback;
    typedef std::shared_ptr<asio::ip::tcp::acceptor> AcceptorPointer;
    typedef std::shared_ptr<ConnectionsManager> Pointer;

    ConnectionsManager(asio::io_service &ioService, int16_t port);
    ConnectionsManager(ConnectionsFactory::Pointer connectionsFactory, asio::io_service &ioService, AcceptorPointer acceptor);

    void queueAccept();

    void setGenerateConnectionHashCallback(const GenerateConnectionHashCallback &callback);
    void setAcceptEventCallback(const AcceptEventCallback &callback);
    void setReceiveEventCallback(const ReceiveEventCallback &callback);
    void setCloseEventCallback(const CloseEventCallback &callback);

    void send(size_t hash, const Payload &payload);
    void disconnect(size_t hash);

private:
    void acceptHandler(Connection::SocketPointer socket, const boost::system::error_code &errorCode);
    void registerConnection(Connection::SocketPointer socket);

    void receiveEvent(Connection &connection);
    void closeEvent(Connection &connection);

    ConnectionsFactory::Pointer connectionsFactory_;
    asio::io_service &ioService_;
    AcceptorPointer acceptor_;

    GenerateConnectionHashCallback generateConnectionHashCallback_;
    AcceptEventCallback acceptEventCallback_;
    ReceiveEventCallback receiveEventCallback_;
    CloseEventCallback closeEventCallback_;
};

}
}
}
}

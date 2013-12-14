#pragma once

#include <core/common/factory.hpp>
#include <core/network/tcp/connection.hpp>
#include <common/asio.hpp>

#include <functional>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class ConnectionsAcceptor
{
public:
    typedef std::function<void(Connection&)> AcceptEventCallback;
    typedef std::shared_ptr<asio::ip::tcp::acceptor> AcceptorPointer;
    typedef std::shared_ptr<ConnectionsAcceptor> Pointer;

    ConnectionsAcceptor(common::Factory<Connection> &connectionsFactory, AcceptorPointer acceptor);
    virtual ~ConnectionsAcceptor();

    void queueAccept();
    void setAcceptEventCallback(const AcceptEventCallback &callback);

private:
    void acceptHandler(Connection::SocketPointer socket, const boost::system::error_code &errorCode);

    common::Factory<Connection> &connectionsFactory_;
    AcceptorPointer acceptor_;
    AcceptEventCallback acceptEventCallback_;
    asio::io_service::strand strand_;
};

}
}
}
}

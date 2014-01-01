#pragma once

#include <core/common/factory.hpp>
#include <core/network/tcp/connection.hpp>
#include <core/network/tcp/protocol.hpp>
#include <core/common/asio.hpp>

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
    ConnectionsAcceptor(asio::io_service &ioService, uint16_t port, Protocol &protocol);
    virtual ~ConnectionsAcceptor();

    void queueAccept();
    asio::ip::tcp::acceptor& getAcceptor();

private:
    void acceptHandler(Connection::Pointer connection, const boost::system::error_code &errorCode);

    asio::ip::tcp::acceptor acceptor_;
    asio::io_service::strand strand_;
    Protocol &protocol_;
};

}
}
}
}

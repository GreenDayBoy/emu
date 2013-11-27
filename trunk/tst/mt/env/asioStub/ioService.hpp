#pragma once

#include <mt/env/asioStub/types.hpp>
#include <mt/env/asioStub/tcp/socket.hpp>
#include <mt/env/asioStub/udp/socket.hpp>

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>

#include <vector>
#include <boost/system/error_code.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

class io_service
{
public:
    class strand
    {
    public:
        strand(io_service& service);

        template<typename CompletionHandler>
        CompletionHandler wrap(const CompletionHandler &handler) { return handler; }
    };

    class AcceptingNotStartedException {};
    class NonExistentTcpSocketException {};
    class NonExistentUdpSocketException {};

    typedef std::vector<ip::tcp::socket*> TcpSocketsContainer;

    io_service();
    ~io_service();

    template<typename CompletionHandler>
    void post(const CompletionHandler &handler) { handler(); }

    size_t run();
    void stop();

    void establishUdpConnection(ip::udp::socket *socket);
    void sendTo(const protocol::WriteStream &writeStream);
    protocol::ReadStream receiveFrom();

    size_t establishTcpConnection();
    void closeTcpConnection(size_t hash);
    void closeTcpConnection(ip::tcp::socket *socket);
    bool tcpConnectionEstablished(size_t hash) const;

    void queueAccept(ip::tcp::socket &socket, const AcceptHandler &handler);
    void send(size_t hash, const protocol::WriteStream &writeStream);
    protocol::ReadStream receive(size_t hash);
    void disconnect(size_t hash);

private:
    TcpSocketsContainer::iterator find(size_t hash);
    void closeAllTcpConnections();

    TcpSocketsContainer tcpSockets_;
    ip::tcp::socket *tcpSocket_;
    AcceptHandler acceptHandler_;

    ip::udp::socket *udpSocket_;
};

}
}
}
}

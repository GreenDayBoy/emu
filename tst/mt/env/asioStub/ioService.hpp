#pragma once

#include <vector>
#include <boost/system/error_code.hpp>
#include <mt/env/asioStub/types.hpp>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

namespace ip
{
namespace tcp
{
class socket;
}
namespace udp
{
class socket;
}
}

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

    typedef std::vector<ip::tcp::socket*> SocketsContainer;

    io_service();

    template<typename CompletionHandler>
    void post(const CompletionHandler &handler) { handler(); }

    size_t run();
    void stop();

    void queueAccept(ip::tcp::socket &socket, const AcceptHandler &handler);
    size_t createConnection();

    void send(size_t hash, const core::network::Payload &payload);
    core::network::Payload receive(size_t hash);

    void close(size_t hash);
    void close(ip::tcp::socket *socket);

    void registerUdpSocket(ip::udp::socket *socket);

    void sendTo(const core::network::Payload &payload);
    core::network::Payload receiveFrom();

    bool exists(size_t hash) const;

private:
    SocketsContainer::iterator find(size_t hash);

    ip::tcp::socket *incomingSocket_;
    AcceptHandler acceptHandler_;

    ip::udp::socket *udpSocket_;

    SocketsContainer sockets_;
};

}
}
}
}

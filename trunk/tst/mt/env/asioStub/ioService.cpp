#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/tcp/socket.hpp>
#include <mt/env/asioStub/udp/socket.hpp>
#include <mt/env/asioStub/exceptions.hpp>
#include <algorithm>
#include <gtest/gtest.h>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

io_service::strand::strand(io_service& service) {}

io_service::io_service():
    incomingSocket_(nullptr),
    udpSocket_(nullptr) {}

size_t io_service::run()
{
    return 0;
}

void io_service::stop() {}

void io_service::queueAccept(ip::tcp::socket &socket, const AcceptHandler &handler)
{
    incomingSocket_ = &socket;
    acceptHandler_ = handler;
}

size_t io_service::createConnection()
{
    sockets_.push_back(incomingSocket_);
    acceptHandler_(boost::system::error_code());

    return reinterpret_cast<size_t>(sockets_.back());
}

void io_service::send(size_t hash, const core::network::Payload &payload)
{
    auto it = this->find(hash);

    if(it == sockets_.end())
    {
        throw exceptions::UnknownSocketException();
    }

    (*it)->insertPayload(payload);
}

core::network::Payload io_service::receive(size_t hash)
{
    auto it = this->find(hash);

    if(it == sockets_.end())
    {
        throw exceptions::UnknownSocketException();
    }

    return (*it)->getPayload();
}

io_service::SocketsContainer::iterator io_service::find(size_t hash)
{
    return std::find_if(sockets_.begin(), sockets_.end(),
                        [hash](ip::tcp::socket *socket) { return hash == reinterpret_cast<size_t>(socket); });
}

void io_service::close(size_t hash)
{
    auto it = this->find(hash);

    if(it == sockets_.end())
    {
        throw exceptions::UnknownSocketException();
    }

    sockets_.erase(it);
}

void io_service::close(ip::tcp::socket *socket)
{
    this->close(reinterpret_cast<size_t>(socket));
}

void io_service::registerUdpSocket(ip::udp::socket *socket)
{
    udpSocket_ = socket;
}

void io_service::sendTo(const core::network::Payload &payload)
{
    if(udpSocket_ == nullptr)
    {
        throw exceptions::NotCreatedUdpSocketException();
    }

    udpSocket_->insertPayload(payload);
}

core::network::Payload io_service::receiveFrom()
{
    if(udpSocket_ == nullptr)
    {
        throw exceptions::NotCreatedUdpSocketException();
    }

    return udpSocket_->getPayload();
}

bool io_service::exists(size_t hash) const
{
    return const_cast<io_service*>(this)->find(hash) != sockets_.end();
}

void io_service::disconnect(size_t hash)
{
    auto it = this->find(hash);

    if(it == sockets_.end())
    {
        throw exceptions::UnknownSocketException();
    }

    (*it)->disconnect();
}

}
}
}
}

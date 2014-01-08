#pragma once

#include <core/network/tcp/connection.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

class Client: boost::noncopyable
{
public:
    Client();
    virtual ~Client();

    void setClientConnection(core::network::tcp::Connection::Pointer connection);
    core::network::tcp::Connection::Pointer getClientConnection();

protected:
    core::network::tcp::Connection::Pointer clientConnection_;
};

}
}
}

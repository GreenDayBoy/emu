#pragma once

#include <core/network/tcp/protocol.hpp>
#include <streaming/readStream.hpp>
#include <protocols/contexts/client.hpp>

namespace eMU
{
namespace protocols
{

class Client: public core::network::tcp::Protocol
{
public:
    Client(contexts::Client &context);

    bool attach(core::network::tcp::Connection::Pointer connection);
    void detach(core::network::tcp::Connection::Pointer connection);
    bool dispatch(core::network::tcp::Connection::Pointer connection);

protected:
    virtual bool handleReadStream(const streaming::ReadStream &stream) = 0;

private:
    contexts::Client &context_;
};

}
}

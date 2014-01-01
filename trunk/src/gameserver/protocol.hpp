#pragma once

#include <core/network/tcp/protocol.hpp>
#include <protocol/readStream.hpp>
#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

class Protocol: public core::network::tcp::Protocol
{
public:
    Protocol(Context &context);

    bool attach(core::network::tcp::Connection::Pointer connection);
    void detach(core::network::tcp::Connection::Pointer connection);
    bool dispatch(core::network::tcp::Connection::Pointer connection);

private:
    bool handleReadStream(User &user, const protocol::ReadStream &stream);

    Context &context_;
};

}
}

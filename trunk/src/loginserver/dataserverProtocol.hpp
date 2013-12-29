#pragma once

#include <core/network/tcp/protocol.hpp>
#include <protocol/readStream.hpp>
#include <loginserver/context.hpp>

namespace eMU
{
namespace loginserver
{

class DataserverProtocol: public core::network::tcp::Protocol
{
public:
    DataserverProtocol(Context &context);

    bool attach(core::network::tcp::Connection::Pointer connection);
    void detach(core::network::tcp::Connection::Pointer connection);
    bool dispatch(core::network::tcp::Connection::Pointer connection);

private:
    void handleReadStream(const protocol::ReadStream &stream);

    Context &context_;
};

}
}

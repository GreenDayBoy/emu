#pragma once

#include <protocols/server.hpp>
#include <streaming/readStream.hpp>
#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

class Protocol: public protocols::Server<User>
{
public:
    Protocol(Context &context);
    bool attach(core::network::tcp::Connection::Pointer connection);

private:
    bool handleReadStream(User &user, const streaming::ReadStream &stream);

    Context &context_;
};

}
}

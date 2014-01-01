#pragma once

#include <protocols/server.hpp>
#include <streaming/readStream.hpp>
#include <loginserver/context.hpp>

namespace eMU
{
namespace loginserver
{

class Protocol: public protocols::Server<User>
{
public:
    Protocol(Context &context);

private:
    bool handleReadStream(User &user, const streaming::ReadStream &stream);

    Context &context_;
};

}
}

#pragma once

#include <protocols/contexts/server.hpp>
#include <protocols/contexts/client.hpp>
#include <protocols/contexts/executive.hpp>

#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{

class Context: public protocols::contexts::Client, public protocols::contexts::Server<User>, public protocols::contexts::Executive
{
public:
    Context(size_t maxNumberOfUsers);

private:
    Context();
};

}
}

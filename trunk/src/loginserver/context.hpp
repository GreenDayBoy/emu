#pragma once

#include <protocols/contexts/server.hpp>
#include <protocols/contexts/client.hpp>
#include <protocols/contexts/executive.hpp>

#include <loginserver/user.hpp>
#include <loginserver/gameserversList.hpp>

namespace eMU
{
namespace loginserver
{

class Context: public protocols::contexts::Client, public protocols::contexts::Server<User>, public protocols::contexts::Executive
{
public:
    Context(size_t maxNumberOfUsers);

    GameserversList& getGameserversList();

private:
    Context();

    GameserversList gameserversList_;
};

}
}

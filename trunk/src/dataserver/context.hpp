#pragma once

#include <protocols/contexts/server.hpp>
#include <protocols/contexts/executive.hpp>

#include <dataserver/user.hpp>
#include <dataserver/database/sqlInterface.hpp>

namespace eMU
{
namespace dataserver
{

class Context: public protocols::contexts::Server<User>, public protocols::contexts::Executive
{
public:
    Context(database::SqlInterface &sqlInterface, size_t maxNumberOfUsers);

    database::SqlInterface& getSqlInterface();

private:
    Context();

    database::SqlInterface &sqlInterface_;
};

}
}

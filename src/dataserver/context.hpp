#pragma once

#include <protocols/contexts/server.hpp>

#include <dataserver/user.hpp>
#include <dataserver/database/sqlInterface.hpp>

namespace eMU
{
namespace dataserver
{

class Context: public protocols::contexts::Server<User>
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

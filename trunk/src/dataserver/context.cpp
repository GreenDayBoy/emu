#include <dataserver/context.hpp>

namespace eMU
{
namespace dataserver
{

Context::Context(database::SqlInterface &sqlInterface, size_t maxNumberOfUsers):
    protocols::contexts::Server<User>(maxNumberOfUsers),
    sqlInterface_(sqlInterface) {}

database::SqlInterface& Context::getSqlInterface()
{
    return sqlInterface_;
}

}
}

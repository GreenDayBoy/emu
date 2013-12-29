#include <core/network/tcp/networkUser.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

NetworkUser::NetworkUser(Connection::Pointer connection):
    connection_(connection) {}

NetworkUser::~NetworkUser() {}

Connection& NetworkUser::getConnection()
{
    return *connection_;
}

bool NetworkUser::operator==(const Connection::Pointer connection) const
{
    return connection_ == connection;
}

bool NetworkUser::operator==(Hash hash) const
{
    return this->getHash() == hash;
}

bool NetworkUser::operator==(const NetworkUser &user) const
{
    return this->getHash() == user.getHash();
}

NetworkUser::Hash NetworkUser::getHash() const
{
    return Hash(reinterpret_cast<size_t>(this));
}

}
}
}
}

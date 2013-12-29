#pragma once

#include <core/network/tcp/networkUser.hpp>

namespace eMU
{
namespace dataserver
{

class User: public core::network::tcp::NetworkUser
{
public:
    User(core::network::tcp::Connection::Pointer connection);

private:
    User();
};

}
}

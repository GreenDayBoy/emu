#pragma once

#include <core/network/tcp/networkUser.hpp>

namespace eMU
{
namespace gameserver
{

class User: public core::network::tcp::NetworkUser
{
public:
    User(core::network::tcp::Connection &connection);

private:
    User();
};

}
}

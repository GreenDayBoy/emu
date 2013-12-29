#pragma once

#include <core/network/tcp/networkUser.hpp>
#include <core/network/payload.hpp>

#include <list>

namespace eMU
{
namespace analyzer
{

class User: public core::network::tcp::NetworkUser
{
public:
    User(core::network::tcp::Connection::Pointer connection);

    void storeReadPayload();
    std::list<core::network::Payload>& getReadPayloads();

private:
    User();

    std::list<core::network::Payload> readPayloads_;
};

}
}

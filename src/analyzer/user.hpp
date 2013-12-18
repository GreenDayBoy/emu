#pragma once

#include <core/network/tcp/networkUser.hpp>
#include <protocol/readStream.hpp>

#include <list>

namespace eMU
{
namespace analyzer
{

class User: public core::network::tcp::NetworkUser
{
public:
    User(core::network::tcp::Connection &connection);

    void parseReadPayload();
    std::list<core::network::Payload>& getReadPayloads();

private:
    User();

    std::list<core::network::Payload> readPayloads_;
};

}
}

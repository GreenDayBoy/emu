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
    std::list<protocol::ReadStream>& getReadStreams();

private:
    User();

    std::list<protocol::ReadStream> readStreams_;
};

}
}

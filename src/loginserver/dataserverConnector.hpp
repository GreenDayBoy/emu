#pragma once

#include <core/network/tcp/connection.hpp>
#include <string>

namespace eMU
{
namespace loginserver
{

class DataserverConnector
{
public:
    struct ConnectInfo
    {
        std::string address_;
        uint16_t port_;
    };

    typedef std::vector<ConnectInfo> ConnectInfoContainer;

    DataserverConnector(core::network::tcp::Connection &connection, const ConnectInfoContainer &connectInfos);

    bool connect();

private:
    core::network::tcp::Connection &connection_;
    ConnectInfoContainer connectInfos_;
};

}
}

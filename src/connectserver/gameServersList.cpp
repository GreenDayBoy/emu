#include <connectserver/gameServersList.hpp>
#include <core/common/exception.hpp>

namespace eMU
{
namespace connectserver
{

void GameServersList::initialize(eMU::core::common::XmlReader &xmlReader)
{
    while(!xmlReader.end())
    {
        GameServerInfo info = {0};

        info.address_ = xmlReader.get<std::string>("server", "address");
        info.code_ = xmlReader.get<uint16_t>("server", "code");
        info.load_ = 0;
        info.name_ = xmlReader.get<std::string>("server", "name");
        info.port_ = xmlReader.get<uint16_t>("server", "port");
        servers_.push_back(info);

        xmlReader.next();
    }
}

const GameServersList::GameServersListContainer &GameServersList::list() const
{
    return servers_;
}

void GameServersList::updateGameServerLoad(uint32_t code, uint32_t load)
{
for(auto &info : servers_)
    {
        if(info.code_ == code)
        {
            info.load_ = load;

            return;
        }
    }

    core::common::Exception exception;
    exception.in() << "Unknown server code: " << code;
    throw exception;
}

}
}

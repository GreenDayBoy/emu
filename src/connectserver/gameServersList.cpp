#include <connectserver/gameServersList.hpp>
#include <core/common/exception.hpp>

namespace eMU
{
namespace connectserver
{

GameServersList::~GameServersList() {}

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

void GameServersList::updateGameServerLoad(uint16_t code, uint32_t load)
{
    for(auto &info : servers_)
    {
        if(info.code_ == code)
        {
            info.load_ = load;

            return;
        }
    }
}

bool GameServersList::hasGameServer(uint16_t code) const
{
    return std::find_if(servers_.begin(),
                        servers_.end(),
                        [code](const GameServerInfo &info) { return info.code_ == code; }) != servers_.end();
}

}
}

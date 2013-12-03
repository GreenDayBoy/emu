#include <loginserver/gameserversList.hpp>
#include <core/common/exception.hpp>

namespace eMU
{
namespace loginserver
{

GameserversList::~GameserversList() {}

void GameserversList::initialize(eMU::core::common::XmlReader &xmlReader)
{
    while(!xmlReader.end())
    {
        protocol::loginserver::GameserverInfo info = {};

        info.address_ = xmlReader.get<std::string>("server", "address");
        info.code_ = xmlReader.get<uint16_t>("server", "code");
        info.load_ = 0;
        info.name_ = xmlReader.get<std::string>("server", "name");
        info.port_ = xmlReader.get<uint16_t>("server", "port");
        servers_.push_back(info);

        xmlReader.next();
    }
}

const protocol::loginserver::GameserversInfoContainer &GameserversList::getServers() const
{
    return servers_;
}

void GameserversList::updateGameserverLoad(uint16_t code, uint32_t load)
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

bool GameserversList::hasGameserver(uint16_t code) const
{
    return findGameserverInfo(code) != servers_.end();
}

const protocol::loginserver::GameserverInfo& GameserversList::getGameserverInfo(uint16_t code) const
{
    return *findGameserverInfo(code);
}

protocol::loginserver::GameserversInfoContainer::const_iterator GameserversList::findGameserverInfo(uint16_t code) const
{
    return std::find_if(servers_.begin(),
                        servers_.end(),
                        [code](const protocol::loginserver::GameserverInfo &info) { return info.code_ == code; });
}

}
}

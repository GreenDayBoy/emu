#include <loginserver/gameserversList.hpp>
#include <core/common/exception.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

GameserversList::~GameserversList() {}

bool GameserversList::initialize(eMU::core::common::XmlReader &xmlReader)
{
    if(!xmlReader.parse("servers"))
    {
        LOG(ERROR) << "Cannot parse servers list xml.";

        return false;
    }

    while(!xmlReader.end())
    {
        streaming::loginserver::GameserverInfo info = {};

        info.address_ = xmlReader.get<std::string>("server", "address");
        info.code_ = xmlReader.get<uint16_t>("server", "code");
        info.name_ = xmlReader.get<std::string>("server", "name");
        info.port_ = xmlReader.get<uint16_t>("server", "port");
        servers_.push_back(info);

        xmlReader.next();
    }

    return true;
}

const streaming::loginserver::GameserversInfoContainer &GameserversList::getServers() const
{
    return servers_;
}

bool GameserversList::hasGameserver(uint16_t code) const
{
    return findGameserverInfo(code) != servers_.end();
}

const streaming::loginserver::GameserverInfo& GameserversList::getGameserverInfo(uint16_t code) const
{
    return *findGameserverInfo(code);
}

streaming::loginserver::GameserversInfoContainer::const_iterator GameserversList::findGameserverInfo(uint16_t code) const
{
    return std::find_if(servers_.begin(),
                        servers_.end(),
                        [code](const streaming::loginserver::GameserverInfo &info) { return info.code_ == code; });
}

}
}

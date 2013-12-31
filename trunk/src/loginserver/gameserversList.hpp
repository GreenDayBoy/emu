#pragma once

#include <core/common/xmlReader.hpp>
#include <common/mockable.hpp>
#include <protocol/loginserver/gameserverInfo.hpp>

#include <stdint.h>
#include <string>

namespace eMU
{
namespace loginserver
{

class GameserversList
{
public:  

    virtual ~GameserversList();
    bool initialize(eMU::core::common::XmlReader &xmlReader);
    MOCKABLE const protocol::loginserver::GameserversInfoContainer& getServers() const;
    MOCKABLE bool hasGameserver(uint16_t code) const;
    MOCKABLE const protocol::loginserver::GameserverInfo& getGameserverInfo(uint16_t code) const;

private:
    protocol::loginserver::GameserversInfoContainer::const_iterator findGameserverInfo(uint16_t code) const;

    protocol::loginserver::GameserversInfoContainer servers_;
};

}
}

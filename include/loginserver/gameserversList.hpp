#pragma once

#include <core/common/xmlReader.hpp>
#include <core/common/mockable.hpp>
#include <streaming/loginserver/gameserverInfo.hpp>

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
    MOCKABLE const streaming::loginserver::GameserversInfoContainer& getServers() const;
    MOCKABLE bool hasGameserver(uint16_t code) const;
    MOCKABLE const streaming::loginserver::GameserverInfo& getGameserverInfo(uint16_t code) const;

private:
    streaming::loginserver::GameserversInfoContainer::const_iterator findGameserverInfo(uint16_t code) const;

    streaming::loginserver::GameserversInfoContainer servers_;
};

}
}

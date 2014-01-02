#pragma once

#include <protocols/contexts/server.hpp>
#include <protocols/contexts/client.hpp>
#include <protocols/contexts/udp.hpp>

#include <gameserver/user.hpp>
#include <streaming/gameserver/userRegistrationInfo.hpp>

namespace eMU
{
namespace gameserver
{

class Context: public protocols::contexts::Client, public protocols::contexts::Server<User>, public protocols::contexts::Udp
{
public:
    Context(size_t maxNumberOfUsers, uint16_t gameserverCode);

    streaming::gameserver::UserRegistrationInfoContainer& getUserRegistrationInfos();
    uint16_t getGameserverCode() const;

private:
    Context();

    uint16_t gameserverCode_;
    streaming::gameserver::UserRegistrationInfoContainer userRegistrationInfos_;
};

}
}

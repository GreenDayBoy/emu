#pragma once

#include <core/common/transaction.hpp>
#include <core/network/tcp/connection.hpp>
#include <streaming/gameserver/worldLoginRequest.hpp>
#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class WorldLoginRequest: public core::common::Transaction
{
public:
    WorldLoginRequest(User &user, const streaming::gameserver::WorldLoginRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    streaming::gameserver::WorldLoginRequest request_;
};

}
}
}

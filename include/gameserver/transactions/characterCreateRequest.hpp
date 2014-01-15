#pragma once

#include <core/common/transaction.hpp>
#include <core/network/tcp/connection.hpp>
#include <streaming/gameserver/characterCreateRequest.hpp>
#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class CharacterCreateRequest: public core::common::Transaction
{
public:
    CharacterCreateRequest(User &user,
                           core::network::tcp::Connection::Pointer dataserverConnection,
                           const streaming::gameserver::CharacterCreateRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    core::network::tcp::Connection::Pointer dataserverConnection_;
    streaming::gameserver::CharacterCreateRequest request_;
};

}
}
}

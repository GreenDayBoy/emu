#pragma once

#include <core/common/transaction.hpp>
#include <core/network/tcp/connection.hpp>
#include <streaming/gameserver/charactersListRequest.hpp>
#include <gameserver/user.hpp>

namespace eMU
{
namespace gameserver
{
namespace transactions
{

class CharactersListRequest: public core::common::Transaction
{
public:
    CharactersListRequest(User &user,
                          core::network::tcp::Connection::Pointer dataserverConnection,
                          const streaming::gameserver::CharactersListRequest &request);

private:
    bool isValid() const;
    void handleValid();
    void handleInvalid();

    User &user_;
    core::network::tcp::Connection::Pointer dataserverConnection_;
    streaming::gameserver::CharactersListRequest request_;
};

}
}
}

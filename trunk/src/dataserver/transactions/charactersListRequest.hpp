#pragma once

#include <dataserver/transactions/databaseTransaction.hpp>
#include <streaming/dataserver/charactersListRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CharactersListRequest: public DatabaseTransaction
{
public:
    CharactersListRequest(User &user,
                          database::SqlInterface &sqlInterface,
                          const streaming::dataserver::CharactersListRequest &request);

private:
    void handleValid();

    streaming::dataserver::CharactersListRequest request_;
};

}
}
}

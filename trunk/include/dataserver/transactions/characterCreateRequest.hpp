#pragma once

#include <dataserver/transactions/databaseTransaction.hpp>
#include <streaming/dataserver/characterCreateRequest.hpp>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

class CharacterCreateRequest: public DatabaseTransaction
{
public:
    CharacterCreateRequest(User &user,
                           database::SqlInterface &sqlInterface,
                           const streaming::dataserver::CharacterCreateRequest &request);

private:
    void handleValid();

    streaming::dataserver::CharacterCreateRequest request_;
};

}
}
}

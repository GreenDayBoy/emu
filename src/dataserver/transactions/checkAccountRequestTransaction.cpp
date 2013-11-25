#include <dataserver/transactions/checkAccountRequestTransaction.hpp>
#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/dataserver/CheckAccountResult.hpp>

#include <sstream>

namespace eMU
{
namespace dataserver
{
namespace transactions
{

CheckAccountRequestTransaction::CheckAccountRequestTransaction(size_t hash,
                                                               database::SqlInterface &sqlInterface,
                                                               core::network::tcp::ConnectionsManager &connectionsManager,
                                                               const protocol::dataserver::decoders::CheckAccountRequest &request):
    hash_(hash),
    sqlInterface_(sqlInterface),
    connectionsManager_(connectionsManager),
    request_(request) {}

bool CheckAccountRequestTransaction::isValid() const
{
    return true;
}

void CheckAccountRequestTransaction::handle()
{
    std::stringstream query;
    query << "SELECT"
          << " `eMU_AccountCheck`(" << request_.getAccountId()
                                    << ", "
                                    << request_.getPassword()
                                    << ");";

    sqlInterface_.executeQuery(query.str());

    const database::QueryResult &queryResult = sqlInterface_.fetchQueryResult();

    if(queryResult.getRows().size() > 0)
    {
        protocol::dataserver::CheckAccountResult result = static_cast<protocol::dataserver::CheckAccountResult>(queryResult.getRows()[0].getValue<uint32_t>(0));
        protocol::dataserver::encoders::CheckAccountResponse response(request_.getClientHash(), result);

        connectionsManager_.send(hash_, response.getWriteStream().getPayload());
    }
}

}
}
}

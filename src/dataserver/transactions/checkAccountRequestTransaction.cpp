//#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

//#include <sstream>

//namespace eMU
//{
//namespace dataserver
//{
//namespace transactions
//{

//CheckAccountRequestTransaction::CheckAccountRequestTransaction(size_t hash, MessageSender &messageSender,
//                                                               const interface::dataserver::decoders::CheckAccountRequest &request,
//                                                               database::SqlInterface &sqlInterface):
//    hash_(hash),
//    messageSender_(messageSender),
//    request_(request),
//    sqlInterface_(sqlInterface) {}

//bool CheckAccountRequestTransaction::isValid() const
//{
//    return true;
//}

//void CheckAccountRequestTransaction::handle()
//{
//    std::stringstream query;
//    query << "SELECT"
//          << " `eMU_AccountCheck`();";

//    sqlInterface_.executeQuery(query.str());

//    const database::QueryResult&& queryResult = sqlInterface_.fetchQueryResult();

//    if(queryResult.getRows().size() > 0)
//    {
//        int32_t checkResult = queryResult.getFieldValue<int32_t>(0);
//        messageSender_.sendCheckAccountResponse(0, checkResult);
//    }
//}

//}
//}
//}

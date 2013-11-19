//#pragma once

//#include <core/transactions/transaction.hpp>
//#include <interface/dataserver/decoders/checkAccountRequest.hpp>
//#include <dataserver/database/sqlInterface.hpp>
//#include <dataserver/messageSender.hpp>

//namespace eMU
//{
//namespace dataserver
//{
//namespace transactions
//{

//class CheckAccountRequestTransaction: public eMU::core::transactions::Transaction
//{
//public:
//    CheckAccountRequestTransaction(size_t hash, MessageSender &messageSender,
//                                   const interface::dataserver::decoders::CheckAccountRequest &request,
//                                   database::SqlInterface &sqlInterface);

//    bool isValid() const;
//    void handle();

//private:
//    void decode();

//    size_t hash_;
//    MessageSender &messageSender_;
//    const interface::dataserver::decoders::CheckAccountRequest &request_;
//    database::SqlInterface &sqlInterface_;
//};

//}
//}
//}

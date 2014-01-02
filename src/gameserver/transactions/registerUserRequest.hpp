//#pragma once

//#include <core/common/transaction.hpp>
//#include <core/network/udp/connection.hpp>
//#include <streaming/gameserver/registerUserRequest.hpp>

//namespace eMU
//{
//namespace gameserver
//{
//namespace transactions
//{

//class RegisterUserRequest: public core::common::Transaction
//{
//public:
//    RegisterUserRequest(const boost::asio::ip::udp::endpoint &sender,
//                             core::network::udp::Connection::Pointer gameserverConnection,
//                             const streaming::gameserver::RegisterUserRequest &request);

//private:
//    bool isValid() const;
//    void handleValid();
//    void handleInvalid();

//    core::network::udp::Connection::Pointer loginserverConnection_;
//    const streaming::gameserver::RegisterUserRequest request_;
//};

//}
//}
//}

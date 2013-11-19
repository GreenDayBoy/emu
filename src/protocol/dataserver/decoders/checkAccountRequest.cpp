//#include <interface/dataserver/decoders/checkAccountRequest.hpp>

//namespace eMU
//{
//namespace interface
//{
//namespace dataserver
//{
//namespace decoders
//{

//CheckAccountRequest::CheckAccountRequest(core::network::Payload &packet)
//{
//    clientHash_ = packet.read<size_t>();

//    size_t accountIdLength = packet.read<uint32_t>();
//    accountId_ = packet.readString(accountIdLength);

//    size_t passwordLength = packet.read<uint32_t>();
//    password_ = packet.readString(passwordLength);
//}

//size_t CheckAccountRequest::getClientHash() const
//{
//    return clientHash_;
//}

//const std::string& CheckAccountRequest::getAccountId() const
//{
//    return accountId_;
//}

//const std::string& CheckAccountRequest::getPassword() const
//{
//    return password_;
//}

//}
//}
//}
//}

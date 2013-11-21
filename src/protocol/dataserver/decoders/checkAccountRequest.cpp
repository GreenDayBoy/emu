#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

CheckAccountRequest::CheckAccountRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<size_t>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    accountId_ = readStream_.readNextString(accountIdLength);

    uint32_t passwordLength = readStream_.readNext<uint32_t>();
    password_ = readStream_.readNextString(passwordLength);
}

CheckAccountRequest::CheckAccountRequest(size_t clientHash, const std::string &accountId, const std::string &password):
    readStream_(core::network::Payload()),
    clientHash_(clientHash),
    accountId_(accountId),
    password_(password) {}

size_t CheckAccountRequest::getClientHash() const
{
    return clientHash_;
}

const std::string& CheckAccountRequest::getAccountId() const
{
    return accountId_;
}

const std::string& CheckAccountRequest::getPassword() const
{
    return password_;
}

}
}
}
}

#include <streaming/dataserver/checkAccountRequest.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CheckAccountRequest::CheckAccountRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    accountId_ = readStream_.readNextString(accountIdLength);

    uint32_t passwordLength = readStream_.readNext<uint32_t>();
    password_ = readStream_.readNextString(passwordLength);
}

CheckAccountRequest::CheckAccountRequest(core::network::tcp::NetworkUser::Hash userHash, const std::string &accountId, const std::string password):
    writeStream_(streamIds::kCheckAccountRequest)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNextString(accountId);

    writeStream_.writeNext<uint32_t>(password.length());
    writeStream_.writeNextString(password);
}

const WriteStream& CheckAccountRequest::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CheckAccountRequest::getUserHash() const
{
    return userHash_;
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

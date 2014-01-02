#include <streaming/gameserver/registerUserRequest.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

RegisterUserRequest::RegisterUserRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    accountId_ = readStream_.readNextString(accountIdLength);
}

RegisterUserRequest::RegisterUserRequest(core::network::tcp::NetworkUser::Hash userHash, const std::string &accountId):
    writeStream_(streamIds::kRegisterUserRequest)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNextString(accountId);
}

const WriteStream& RegisterUserRequest::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash RegisterUserRequest::getUserHash() const
{
    return userHash_;
}

std::string RegisterUserRequest::getAccountId() const
{
    return accountId_;
}

}
}
}

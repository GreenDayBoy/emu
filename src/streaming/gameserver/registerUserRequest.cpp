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
    userInfo_.userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    userInfo_.accountId_ = readStream_.readNextString(accountIdLength);
}

RegisterUserRequest::RegisterUserRequest(const UserRegistrationInfo &userInfo):
    writeStream_(streamIds::kRegisterUserRequest)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userInfo.userHash_);

    writeStream_.writeNext<uint32_t>(userInfo.accountId_.length());
    writeStream_.writeNextString(userInfo.accountId_);
}

const WriteStream& RegisterUserRequest::getWriteStream() const
{
    return writeStream_;
}

const UserRegistrationInfo& RegisterUserRequest::getUserRegistrationInfo() const
{
    return userInfo_;
}

}
}
}

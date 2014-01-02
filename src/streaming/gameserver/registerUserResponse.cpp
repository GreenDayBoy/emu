#include <streaming/gameserver/registerUserResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

RegisterUserResponse::RegisterUserResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    gameserverCode_ = readStream_.readNext<uint16_t>();
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();
    result_ = readStream_.readNext<UserRegistrationResult>();
}

RegisterUserResponse::RegisterUserResponse(uint16_t gameserverCode, core::network::tcp::NetworkUser::Hash userHash, UserRegistrationResult result):
    writeStream_(streamIds::kRegisterUserResponse)
{
    writeStream_.writeNext<uint16_t>(gameserverCode);
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);
    writeStream_.writeNext<UserRegistrationResult>(result);
}

uint16_t RegisterUserResponse::getGameserverCode() const
{
    return gameserverCode_;
}

core::network::tcp::NetworkUser::Hash RegisterUserResponse::getUserHash() const
{
    return userHash_;
}

UserRegistrationResult RegisterUserResponse::getResult() const
{
    return result_;
}

const WriteStream& RegisterUserResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}

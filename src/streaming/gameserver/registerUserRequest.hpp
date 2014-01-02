#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/gameserver/userRegistrationInfo.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class RegisterUserRequest
{
public:
    RegisterUserRequest(const ReadStream &readStream);
    RegisterUserRequest(const UserRegistrationInfo &userInfo);

    const WriteStream& getWriteStream() const;

    const UserRegistrationInfo& getUserRegistrationInfo() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    UserRegistrationInfo userInfo_;
};

}
}
}

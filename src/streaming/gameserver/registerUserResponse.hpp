#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/gameserver/userRegistrationResult.hpp>
#include <core/network/tcp/networkUser.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class RegisterUserResponse
{
public:
    RegisterUserResponse(const ReadStream &readStream);
    RegisterUserResponse(uint16_t gameserverCode, core::network::tcp::NetworkUser::Hash userHash, UserRegistrationResult result);

    const WriteStream& getWriteStream() const;

    uint16_t getGameserverCode() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    UserRegistrationResult getResult() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    uint16_t gameserverCode_;
    core::network::tcp::NetworkUser::Hash userHash_;
    UserRegistrationResult result_;
};

}
}
}
